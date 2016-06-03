#include "geometry.h"

geom_vertex_t *newVertexFromPoint(point_t *point)
{
	geom_vertex_t *v = newObject(geom_vertex_t);
	v->p = point;
	v->edges = NULL;
	v->geometries = NULL;
	v->evolution = NULL;
	return v;
}

int _vertexIsInEdge(const geom_vertex_t *vertex, const geom_edge_t *edge)
{
	if (edge->s1 == vertex)
	{
		return 1;
	}
	if (edge->s2 == vertex)
	{
		return 2;
	}
	return 0;
}

int _vertexDirectionFromEdge(const geom_vertex_t *vertex, const geom_edge_t *edge)
{ // -1 for left, 1 for right, 0 otherwise
	float edgeVpos[3], otherVpos[3], diff[3];

	if (_vertexIsInEdge(vertex, edge))
	{
		return 0;
	}

	point2Vec(vertex->p, otherVpos);
	point2Vec(edge->s1->p, edgeVpos);
	Vector.subtract(diff, otherVpos, edgeVpos);
	Vector.normalize(diff, diff);
	
	return Math.sign(Vector.dotProduct(diff, edge->right));
}

int _vertexIsOnEdgeLine(geom_vertex_t *vertex, geom_edge_t *edge)
{
	return !_vertexDirectionFromEdge(vertex, edge);
}

geometry_t *_geometryOnSideFromEdge(geom_edge_t *edge, int direction)
{
	if (direction > 0)
	{
		return GEOM_RIGHT(edge);
	}
	if (direction < 0)
	{
		return GEOM_LEFT(edge);
	}
	return NULL;
}

geometry_t *getGeometryOnOtherSideOfEdge(geom_edge_t *edge, geometry_t *geometry)
{
	if (geometry == edge->geomLeft)
	{
		return GEOM_RIGHT(edge);
	}
	else if (geometry == edge->geomRight)
	{
		return GEOM_LEFT(edge);
	}
	return NULL;
}

geom_vertex_t *getOtherVertexOnEdge(geom_edge_t *edge, geom_vertex_t *vertex)
{
	if (vertex == edge->s1)
	{
		return edge->s2;
	}
	else if (vertex == edge->s2)
	{
		return edge->s1;
	}
	return NULL;
}

bool _vertexIsInGeometry(geom_vertex_t *vertex, geometry_t *geometry)
{
	bool isInGeometry = true;
	
	uint i;
	for (i = 0; i < geometry->edges.size && isInGeometry; i++)
	{
		geom_edge_t *edge = (geom_edge_t*)geometry->edges.content[i];
		isInGeometry = (geometry == _geometryOnSideFromEdge(edge, _vertexDirectionFromEdge(vertex, edge)));
	}

	return isInGeometry;
}

void _precalcEdgeData(geom_edge_t *edge)
{
	float p1pos[3], p2pos[3];
	point2Vec(edge->s1->p, p1pos);
	point2Vec(edge->s2->p, p2pos);
	
	Vector.subtract(edge->direction, p2pos, p1pos);
	Vector.normalize(edge->direction, edge->direction);
	
	Vector.set(edge->right, edge->direction[2], 0.0f, -edge->direction[0]);
	Vector.set(edge->left, -edge->direction[2], 0.0f, edge->direction[0]);
}

geom_edge_t *newEdgeWithVertices(geom_vertex_t *v1, geom_vertex_t *v2)
{
	geom_edge_t *e = newObject(geom_edge_t);
	
	e->s1 = v1;
	e->s2 = v2;
	e->geomLeft = NULL;
	e->geomRight = NULL;

	e->edgeVertex = NULL;

	_precalcEdgeData(e);

	List.add(&v1->edges, e);
	List.add(&v2->edges, e);

	return e;
}

geom_edge_t *_edgeBetweenVertices(geom_vertex_t *v1, geom_vertex_t *v2)
{
	list_t *cursor = v1->edges;
	while (cursor)
	{
		geom_edge_t *edge = (geom_edge_t*)cursor->content;
		if (_vertexIsInEdge(v2, edge))
		{
			return edge;
		}
		cursor = cursor->next;
	}
	return NULL;
}

geom_edge_t *newEdgeIfItDoesNotExistYet(geom_vertex_t *v1, geom_vertex_t *v2)
{
	geom_edge_t *edge = _edgeBetweenVertices(v1, v2);
	
	if (edge)
	{
		return edge;
	}
	else
	{
		return newEdgeWithVertices(v1, v2);
	}
}

void _linkNewGeometryToVertex(geometry_t *geom, geom_vertex_t *vertex)
{
	if (!List.contains(&vertex->geometries, geom))
	{
		List.add(&vertex->geometries, geom);
	}
}

geom_vertex_t *getVertexOfTriangleThatIsNotOnEdge(geometry_t *geom, geom_edge_t *edge)
{
	uint i;
	for (i = 0; i < geom->edges.size; i++)
	{
		geom_edge_t *thisEdge = (geom_edge_t*)geom->edges.content[i];
		
		if (thisEdge->s1 != edge->s1 && thisEdge->s1 != edge->s2)
		{
			return thisEdge->s1;
		}

		if (thisEdge->s2 != edge->s1 && thisEdge->s2 != edge->s2)
		{
			return thisEdge->s2;
		}
	}

	return NULL;
}

bool _geometriesAreEqual(geometry_t *a, geometry_t *b)
{
	uint i;
	for (i = 0; i < a->uniqueVertices.size; i++)
	{
		if (Array.indexOf(&b->uniqueVertices, a->uniqueVertices.content[i]) < 0)
		{
			return false;
		}
	}
	return (a->uniqueVertices.size == b->uniqueVertices.size);
}

bool _linkNewGeometryToEdge(geometry_t *geom, geom_edge_t *edge, geom_edge_t *otherEdge)
{ // Returns false if the edge is already linked to two different geometries or to this geometry, true otherwise
	float edgeVec[3], otherVec[3], normal[3];
	geom_vertex_t *otherVertex = NULL;
	float vert1pos[3], vert2pos[3], vert3pos[3];

	if (otherEdge->s1 == edge->s1 || otherEdge->s1 == edge->s2)
	{
		otherVertex = otherEdge->s2;
	}
	else
	{
		otherVertex = otherEdge->s1;
	}

	point2Vec(edge->s1->p, vert1pos);
	point2Vec(edge->s2->p, vert2pos);
	point2Vec(otherVertex->p, vert3pos);

	Vector.subtract(edgeVec, vert2pos, vert1pos);
	Vector.subtract(otherVec, vert3pos, vert1pos);

	// Rotate the vector 90° right
	Vector.set(normal, edgeVec[2], 0.0f, -edgeVec[0]);

	/*if (Vector.dotProduct(normal, otherVec) > 0.0f)
	{
		edge->geomRight = geom;
	}
	else
	{
		edge->geomLeft = geom;
	}*/

	if (edge->geomRight)
	{
		if (_geometriesAreEqual(GEOM_RIGHT(edge), geom))
		{
			return false;
		}
	}

	if (edge->geomLeft)
	{
		if (_geometriesAreEqual(GEOM_LEFT(edge), geom))
		{
			return false;
		}
	}

	if (!edge->geomRight)
	{
		edge->geomRight = geom;
	}
	else if (!edge->geomLeft)
	{
		edge->geomLeft = geom;
	}
	else
	{
		assert(0);
		return false;
	}

	edge->catmullEdgePoint.x = 0.0f;
	edge->catmullEdgePoint.y = 0.0f;
	edge->catmullEdgePoint.z = 0.0f;

	edge->loopEdgePoint.x = 0.0f;
	edge->loopEdgePoint.y = 0.0f;
	edge->loopEdgePoint.z = 0.0f;

	if (edge->geomLeft && edge->geomRight)
	{
		{
			point_t *leftFacePoint = &GEOM_LEFT(edge)->facePoint;
			point_t *rightFacePoint = &GEOM_RIGHT(edge)->facePoint;
		
			Vector.add((float*)&edge->catmullEdgePoint, (float*)edge->s1->p, (float*)edge->s2->p);

			edge->catmullEdgePoint.x += leftFacePoint->x + rightFacePoint->x;
			edge->catmullEdgePoint.y += leftFacePoint->y + rightFacePoint->y;
			edge->catmullEdgePoint.z += leftFacePoint->z + rightFacePoint->z;

			edge->catmullEdgePoint.x /= 4.0f;
			edge->catmullEdgePoint.y /= 4.0f;
			edge->catmullEdgePoint.z /= 4.0f;
		}

		{
			point_t *leftFacePoint = getVertexOfTriangleThatIsNotOnEdge(GEOM_LEFT(edge), edge)->p;
			point_t *rightFacePoint = getVertexOfTriangleThatIsNotOnEdge(GEOM_RIGHT(edge), edge)->p;
			
			Vector.add((float*)&edge->loopEdgePoint, (float*)edge->s1->p, (float*)edge->s2->p);
			Vector.scale((float*)&edge->loopEdgePoint, 3.0f / 8.0f, (float*)&edge->loopEdgePoint);

			edge->loopEdgePoint.x += (leftFacePoint->x + rightFacePoint->x) / 8.0f;
			edge->loopEdgePoint.y += (leftFacePoint->y + rightFacePoint->y) / 8.0f;
			edge->loopEdgePoint.z += (leftFacePoint->z + rightFacePoint->z) / 8.0f;
		}
	}

	_linkNewGeometryToVertex(geom, edge->s1);
	_linkNewGeometryToVertex(geom, edge->s2);

	return true;
}

void _precalcGeometryData(geometry_t *geometry)
{
	uint i = 0;

	Array.init(&geometry->uniqueVertices, geometry->edges.size);

	while (geometry->uniqueVertices.size < geometry->uniqueVertices.capacity)
	{
		geom_edge_t *edge = (geom_edge_t*)geometry->edges.content[i % geometry->uniqueVertices.capacity];
		
		if (Array.indexOf(&geometry->uniqueVertices, edge->s1) < 0)
		{
			Array.add(&geometry->uniqueVertices, edge->s1);
		}

		if (Array.indexOf(&geometry->uniqueVertices, edge->s2) < 0)
		{
			Array.add(&geometry->uniqueVertices, edge->s2);
		}
		
		i++;
	}

	geometry->facePoint.x = 0.0f;
	geometry->facePoint.y = 0.0f;
	geometry->facePoint.z = 0.0f;

	for (i = 0; i < geometry->uniqueVertices.size; i++)
	{
		geom_vertex_t *vertex = (geom_vertex_t*)geometry->uniqueVertices.content[i];
		geometry->facePoint.x += vertex->p->x;
		geometry->facePoint.y += vertex->p->y;
		geometry->facePoint.z += vertex->p->z;
	}

	geometry->facePoint.x /= geometry->uniqueVertices.size;
	geometry->facePoint.y /= geometry->uniqueVertices.size;
	geometry->facePoint.z /= geometry->uniqueVertices.size;
}

geometry_t *newGeometryWithEdges(array_t edges)
{
	uint i;
	geometry_t *geom = newObject(geometry_t);
	bool success = true;

	geom->edges = edges;

	geom->faceVertex = NULL;

	_precalcGeometryData(geom);

	for (i = 0; i < geom->edges.size && success; i++)
	{
		success = _linkNewGeometryToEdge(geom, (geom_edge_t*)geom->edges.content[i], (geom_edge_t*)geom->edges.content[(i+1)%geom->edges.size]);
	}

	if (!success)
	{
		destroy(geom->uniqueVertices.content);
		destroy(geom);
		return NULL;
	}

	Vector.set(geom->color,
		Math.random(),
		Math.random(),
		Math.random());

	return geom;
}

geom_edge_t *_nextEdgeInGeometry(geometry_t *geometry, geom_edge_t *currentEdge)
{
	uint i;
	for (i = 0; i < geometry->edges.size; i++)
	{
		if (geometry->edges.content[i] == currentEdge)
		{
			return (geom_edge_t*)geometry->edges.content[(i+1)%geometry->edges.size];
		}
	}
	return NULL;
}

void _changeGeometryNearEdge(geom_edge_t *edge, geometry_t *oldGeometry, geometry_t *newGeometry)
{
	if (edge->geomLeft == oldGeometry)
	{
		edge->geomLeft = newGeometry;
	}
	else if (edge->geomRight == oldGeometry)
	{
		edge->geomRight = newGeometry;
	}
}

void _changeEdgeOfGeometry(geometry_t *geometry, geom_edge_t *oldEdge, geom_edge_t *newEdge)
{
	uint i;
	for (i = 0; i < geometry->edges.size; i++)
	{
		if (geometry->edges.content[i] == oldEdge)
		{
			geometry->edges.content[i] = newEdge;
		}
	}
}

geom_edge_t *_getEdgeOfGeometryThatGoesThroughVertexButIsntOtherEdge(geometry_t *geometry, geom_vertex_t *vertex, geom_edge_t *edge)
{
	uint i;
	for (i = 0; i < geometry->edges.size; i++)
	{
		if (edge != geometry->edges.content[i] && _vertexIsInEdge(vertex, (geom_edge_t*)geometry->edges.content[i]))
		{
			return (geom_edge_t*)geometry->edges.content[i];
		}
	}
	return NULL;
}

void getEdgesOfGeometryThatGoThroughVertex(const geometry_t *geometry, const geom_vertex_t *vertex, geom_edge_t **edge1, geom_edge_t **edge2)
{
	uint i;

	*edge1 = NULL;
	*edge2 = NULL;

	for (i = 0; i < geometry->edges.size; i++)
	{
		if (_vertexIsInEdge(vertex, (geom_edge_t*)geometry->edges.content[i]))
		{
			if (!*edge1)
			{
				*edge1 = (geom_edge_t*)geometry->edges.content[i];
			}
			else if (!*edge2)
			{
				*edge2 = (geom_edge_t*)geometry->edges.content[i];
			}
			else
			{
				assert(0);
			}
		}
	}
}

point_t *_newPoint(float x, float y, float z)
{
	point_t *p = newObject(point_t);
	p->x = x;
	p->y = y;
	p->z = z;
	return p;
}

list_t *geometriesForCube(void)
{
	list_t *geometries = NULL;
	array_t edgeArray;

	point_t *p1 = _newPoint(-0.5f, 0.5f, -0.5f);
	point_t *p2 = _newPoint(-0.5f, -0.5f, -0.5f);
	point_t *p3 = _newPoint(0.5f, -0.5f, -0.5f);
	point_t *p4 = _newPoint(0.5f, 0.5f, -0.5f);
	point_t *p5 = _newPoint(-0.5f, 0.5f, 0.5f);
	point_t *p6 = _newPoint(-0.5f, -0.5f, 0.5f);
	point_t *p7 = _newPoint(0.5f, -0.5f, 0.5f);
	point_t *p8 = _newPoint(0.5f, 0.5f, 0.5f);

	geom_vertex_t *v1 = newVertexFromPoint(p1);
	geom_vertex_t *v2 = newVertexFromPoint(p2);
	geom_vertex_t *v3 = newVertexFromPoint(p3);
	geom_vertex_t *v4 = newVertexFromPoint(p4);
	geom_vertex_t *v5 = newVertexFromPoint(p5);
	geom_vertex_t *v6 = newVertexFromPoint(p6);
	geom_vertex_t *v7 = newVertexFromPoint(p7);
	geom_vertex_t *v8 = newVertexFromPoint(p8);

	// Bottom edges
	geom_edge_t *e1to2 = newEdgeWithVertices(v1, v2);
	geom_edge_t *e2to3 = newEdgeWithVertices(v2, v3);
	geom_edge_t *e3to4 = newEdgeWithVertices(v3, v4);
	geom_edge_t *e4to1 = newEdgeWithVertices(v4, v1);

	// Top edges
	geom_edge_t *e5to6 = newEdgeWithVertices(v5, v6);
	geom_edge_t *e6to7 = newEdgeWithVertices(v6, v7);
	geom_edge_t *e7to8 = newEdgeWithVertices(v7, v8);
	geom_edge_t *e8to5 = newEdgeWithVertices(v8, v5);

	// Vertical edges
	geom_edge_t *e1to5 = newEdgeWithVertices(v1, v5);
	geom_edge_t *e2to6 = newEdgeWithVertices(v2, v6);
	geom_edge_t *e3to7 = newEdgeWithVertices(v3, v7);
	geom_edge_t *e4to8 = newEdgeWithVertices(v4, v8);

	// Diagonal edges
	geom_edge_t *e3to1 = newEdgeWithVertices(v3, v1); // Bottom
	geom_edge_t *e7to5 = newEdgeWithVertices(v7, v5); // Top
	geom_edge_t *e5to2 = newEdgeWithVertices(v5, v2); // Left
	geom_edge_t *e6to3 = newEdgeWithVertices(v6, v3); // Back
	geom_edge_t *e7to4 = newEdgeWithVertices(v7, v4); // Right
	geom_edge_t *e4to5 = newEdgeWithVertices(v4, v5); // Front

	geometry_t *bottom1, *bottom2;
	geometry_t *top1, *top2;
	geometry_t *front1, *front2;
	geometry_t *back1, *back2;
	geometry_t *right1, *right2;
	geometry_t *left1, *left2;

	// Bottom
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e1to2);
	Array.add(&edgeArray, e2to3);
	Array.add(&edgeArray, e3to1);
	bottom1 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, bottom1);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e3to1);
	Array.add(&edgeArray, e3to4);
	Array.add(&edgeArray, e4to1);
	bottom2 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, bottom2);

	Vector.set(bottom1->color, 1.0f, 0.0f, 0.0f);
	Vector.set(bottom2->color, 1.0f, 1.0f, 0.0f);

	// Top
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e5to6);
	Array.add(&edgeArray, e6to7);
	Array.add(&edgeArray, e7to5);
	top1 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, top1);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e7to5);
	Array.add(&edgeArray, e7to8);
	Array.add(&edgeArray, e8to5);
	top2 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, top2);

	Vector.set(top1->color, 0.0f, 0.0f, 1.0f);
	Vector.set(top2->color, 1.0f, 0.0f, 0.0f);

	// Left
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e1to5);
	Array.add(&edgeArray, e5to2);
	Array.add(&edgeArray, e1to2);
	left1 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, left1);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e5to6);
	Array.add(&edgeArray, e5to2);
	Array.add(&edgeArray, e2to6);
	left2 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, left2);

	Vector.set(left1->color, 0.0f, 0.0f, 1.0f);
	Vector.set(left2->color, 1.0f, 1.0f, 0.0f);

	// Right
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e3to7);
	Array.add(&edgeArray, e7to4);
	Array.add(&edgeArray, e3to4);
	right1 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, right1);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e7to8);
	Array.add(&edgeArray, e4to8);
	Array.add(&edgeArray, e7to4);
	right2 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, right2);

	Vector.set(right1->color, 0.0f, 1.0f, 0.0f);
	Vector.set(right2->color, 1.0f, 1.0f, 0.0f);

	// Front
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e4to1);
	Array.add(&edgeArray, e1to5);
	Array.add(&edgeArray, e4to5);
	front1 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, front1);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e4to5);
	Array.add(&edgeArray, e4to8);
	Array.add(&edgeArray, e8to5);
	front2 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, front2);

	Vector.set(front1->color, 0.0f, 0.0f, 1.0f);
	Vector.set(front2->color, 0.0f, 1.0f, 0.0f);

	// Back
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e2to6);
	Array.add(&edgeArray, e2to3);
	Array.add(&edgeArray, e6to3);
	back1 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, back1);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e6to3);
	Array.add(&edgeArray, e3to7);
	Array.add(&edgeArray, e6to7);
	back2 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, back2);

	Vector.set(back1->color, 0.0f, 1.0f, 0.0f);
	Vector.set(back2->color, 1.0f, 0.0f, 0.0f);

	return geometries;
}

list_t *geometriesForPyramid(void)
{
	list_t *geometries = NULL;
	array_t edgeArray;

	point_t *p1 = _newPoint(Math.cos(0.0f), Math.sin(0.0f), -0.5f);
	point_t *p2 = _newPoint(Math.cos(2 * Math.pi / 3.0f), Math.sin(2 * Math.pi / 3.0f), -0.5f);
	point_t *p3 = _newPoint(Math.cos(4 * Math.pi / 3.0f), Math.sin(4 * Math.pi / 3.0f), -0.5f);
	point_t *p4 = _newPoint(0.0f, 0.0f, 0.5f);

	geom_vertex_t *v1 = newVertexFromPoint(p1);
	geom_vertex_t *v2 = newVertexFromPoint(p2);
	geom_vertex_t *v3 = newVertexFromPoint(p3);
	geom_vertex_t *v4 = newVertexFromPoint(p4);

	// Bottom edges
	geom_edge_t *e1to2 = newEdgeWithVertices(v1, v2);
	geom_edge_t *e2to3 = newEdgeWithVertices(v2, v3);
	geom_edge_t *e3to1 = newEdgeWithVertices(v3, v1);

	// Top edges
	geom_edge_t *e1to4 = newEdgeWithVertices(v1, v4);
	geom_edge_t *e2to4 = newEdgeWithVertices(v2, v4);
	geom_edge_t *e3to4 = newEdgeWithVertices(v3, v4);

	geometry_t *bottom;
	geometry_t *side1, *side2, *side3;

	// Bottom
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e1to2);
	Array.add(&edgeArray, e2to3);
	Array.add(&edgeArray, e3to1);
	bottom = newGeometryWithEdges(edgeArray);
	List.add(&geometries, bottom);
	Vector.set(bottom->color, 1.0f, 0.0f, 0.0f);

	// Top
	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e1to2);
	Array.add(&edgeArray, e1to4);
	Array.add(&edgeArray, e2to4);
	side1 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, side1);
	Vector.set(side1->color, 0.0f, 1.0f, 0.0f);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e2to3);
	Array.add(&edgeArray, e3to4);
	Array.add(&edgeArray, e2to4);
	side2 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, side2);
	Vector.set(side2->color, 0.0f, 0.0f, 1.0f);

	Array.init(&edgeArray, 3);
	Array.add(&edgeArray, e3to1);
	Array.add(&edgeArray, e1to4);
	Array.add(&edgeArray, e3to4);
	side3 = newGeometryWithEdges(edgeArray);
	List.add(&geometries, side3);
	Vector.set(side3->color, 1.0f, 1.0f, 0.0f);

	return geometries;
}
