#include "subdivision.h"

void subdivideCatmull(list_t **geometries)
{
	list_t *newGeometries = NULL;
	list_t **geometryIterator = geometries;
	while (*geometryIterator)
	{
		geometry_t *geometry = (geometry_t*)(*geometryIterator)->content;
		geom_vertex_t *center = newVertexFromPoint(newObject(point_t));
		uint i;

		*center->p = geometry->facePoint;

		// Loop 1, calculate v'
		for (i = 0; i < geometry->uniqueVertices.size; i++)
		{
			geom_vertex_t *vertex = (geom_vertex_t*)geometry->uniqueVertices.content[i];
			if (!vertex->evolution)
			{
				point_t Q;
				point_t R;
				int n = List.count(&vertex->edges);
				int geomCount = List.count(&vertex->geometries);
				point_t *result = newObject(point_t);
				list_t **listIterator = &vertex->geometries;

				Q.x = 0.0f;
				Q.y = 0.0f;
				Q.z = 0.0f;

				R.x = 0.0f;
				R.y = 0.0f;
				R.z = 0.0f;

				while (*listIterator)
				{
					geometry_t *otherGeometry = (geometry_t*)(*listIterator)->content;

					Q.x += otherGeometry->facePoint.x;
					Q.y += otherGeometry->facePoint.y;
					Q.z += otherGeometry->facePoint.z;

					listIterator = &(*listIterator)->next;
				}

				Q.x /= geomCount;
				Q.y /= geomCount;
				Q.z /= geomCount;
			
				listIterator = &vertex->edges;
				while (*listIterator)
				{
					geom_edge_t *edge = (geom_edge_t*)(*listIterator)->content;

					R.x += edge->catmullEdgePoint.x;
					R.y += edge->catmullEdgePoint.y;
					R.z += edge->catmullEdgePoint.z;

					listIterator = &(*listIterator)->next;
				}

				R.x /= n;
				R.y /= n;
				R.z /= n;

				result->x = Q.x / n + 2 * R.x / n + (n - 3) * vertex->p->x / n;
				result->y = Q.y / n + 2 * R.y / n + (n - 3) * vertex->p->y / n;
				result->z = Q.z / n + 2 * R.z / n + (n - 3) * vertex->p->z / n;

				vertex->evolution = newVertexFromPoint(result);
			}
		}

		// Loop 2, create the new shapes
		for (i = 0; i < geometry->uniqueVertices.size; i++)
		{
			geom_vertex_t *vertex = (geom_vertex_t*)geometry->uniqueVertices.content[i];

			geom_edge_t *edge1, *edge2;
			array_t edgeArray;

			getEdgesOfGeometryThatGoThroughVertex(geometry, vertex, &edge1, &edge2);

			assert(edge1 && edge2);

			if (!edge1->edgeVertex)
			{
				edge1->edgeVertex = newVertexFromPoint(newObject(point_t));
				*edge1->edgeVertex->p = edge1->catmullEdgePoint;
			}
			if (!edge2->edgeVertex)
			{
				edge2->edgeVertex = newVertexFromPoint(newObject(point_t));
				*edge2->edgeVertex->p = edge2->catmullEdgePoint;
			}

			Array.init(&edgeArray, 4);
			Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(center, edge1->edgeVertex));
			Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(edge1->edgeVertex, vertex->evolution));
			Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(vertex->evolution, edge2->edgeVertex));
			Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(edge2->edgeVertex, center));

			List.add(&newGeometries, newGeometryWithEdges(edgeArray));
		}

		geometryIterator = &(*geometryIterator)->next;
	}

	List.clear(geometries); // TODO: Free geometries
	*geometries = newGeometries;
}

void subdivideLoop(list_t **geometries)
{
	list_t *newGeometries = NULL;
	list_t **geometryIterator = geometries;
	while (*geometryIterator)
	{
		geometry_t *geometry = (geometry_t*)(*geometryIterator)->content;
		uint i;

		if (geometry->edges.size != 3)
		{
			List.clear(&newGeometries);
			return;
		}

		// Loop 1, calculate v'
		for (i = 0; i < geometry->uniqueVertices.size; i++)
		{
			geom_vertex_t *vertex = (geom_vertex_t*)geometry->uniqueVertices.content[i];
			if (!vertex->evolution)
			{
				list_t **listIterator = &vertex->edges;
				int n = List.count(&vertex->edges);
				float alpha;

				if (n == 3)
				{
					alpha = 3.0f / 16.0f;
				}
				else
				{
					alpha = ((5.0f / 8.0f) - Math.pow((3.0f / 8.0f + Math.cos(2.0f * Math.pi / n) / 4.0f), 2)) / n;
				}

				vertex->evolution = newVertexFromPoint(newObject(point_t));
				Vector.clear((float*)vertex->evolution->p);

				while (*listIterator)
				{
					geom_edge_t *edge = (geom_edge_t*)(*listIterator)->content;

					geom_vertex_t *otherVertex = getOtherVertexOnEdge(edge, vertex);

					Vector.add((float*)vertex->evolution->p, (float*)vertex->evolution->p, (float*)otherVertex->p);

					listIterator = &(*listIterator)->next;
				}

				Vector.scale((float*)vertex->evolution->p, alpha, (float*)vertex->evolution->p);
				Vector.multiplyAdd((float*)vertex->evolution->p, (float*)vertex->evolution->p, 1.0f - n * alpha, (float*)vertex->p);
			}
		}

		// Loop 2, create the edge vertices if necessary
		for (i = 0; i < geometry->edges.size; i++)
		{
			geom_edge_t *edge = (geom_edge_t*)geometry->edges.content[i];

			if (!edge->edgeVertex)
			{
				edge->edgeVertex = newVertexFromPoint(newObject(point_t));
				*edge->edgeVertex->p = edge->loopEdgePoint;
			}
		}

		// Now create the new shapes
		{
			geom_edge_t **edges = (geom_edge_t**)geometry->edges.content;
			array_t edgeArray;

			for (i = 0; i < geometry->uniqueVertices.size; i++)
			{
				geom_vertex_t *vertex = (geom_vertex_t*)geometry->uniqueVertices.content[i];
				geom_edge_t *edge1, *edge2;

				getEdgesOfGeometryThatGoThroughVertex(geometry, vertex, &edge1, &edge2);

				Array.init(&edgeArray, 3);
				Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(edge1->edgeVertex, edge2->edgeVertex));
				Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(edge2->edgeVertex, vertex->evolution));
				Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(vertex->evolution, edge1->edgeVertex));
				List.add(&newGeometries, newGeometryWithEdges(edgeArray));
			}

			Array.init(&edgeArray, 3);
			Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(edges[0]->edgeVertex, edges[1]->edgeVertex));
			Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(edges[1]->edgeVertex, edges[2]->edgeVertex));
			Array.add(&edgeArray, newEdgeIfItDoesNotExistYet(edges[2]->edgeVertex, edges[0]->edgeVertex));
			List.add(&newGeometries, newGeometryWithEdges(edgeArray));
		}


		geometryIterator = &(*geometryIterator)->next;
	}

	List.clear(geometries); // TODO: Free geometries
	*geometries = newGeometries;
}

void subdivideKobbelt(list_t **geometries)
{
	list_t *newGeometries = NULL;
	list_t **geometryIterator = geometries;
	while (*geometryIterator)
	{
		geometry_t *geometry = (geometry_t*)(*geometryIterator)->content;
		uint i;

		if (!geometry->faceVertex)
		{
			geometry->faceVertex = newVertexFromPoint(newObject(point_t));
			*geometry->faceVertex->p = geometry->facePoint;
		}

		if (geometry->edges.size != 3)
		{
			List.clear(&newGeometries);
			return;
		}

		// Loop 1, calculate v'
		for (i = 0; i < geometry->uniqueVertices.size; i++)
		{
			geom_vertex_t *vertex = (geom_vertex_t*)geometry->uniqueVertices.content[i];
			if (!vertex->evolution)
			{
				list_t **listIterator = &vertex->edges;
				int n = List.count(&vertex->edges);
				float alpha = (4.0f - 2.0f * Math.cos(2.0f * Math.pi / n)) / 9.0f;

				vertex->evolution = newVertexFromPoint(newObject(point_t));
				Vector.clear((float*)vertex->evolution->p);

				while (*listIterator)
				{
					geom_edge_t *edge = (geom_edge_t*)(*listIterator)->content;

					geom_vertex_t *otherVertex = getOtherVertexOnEdge(edge, vertex);

					Vector.add((float*)vertex->evolution->p, (float*)vertex->evolution->p, (float*)otherVertex->p);

					listIterator = &(*listIterator)->next;
				}

				Vector.scale((float*)vertex->evolution->p, alpha / n, (float*)vertex->evolution->p);
				Vector.multiplyAdd((float*)vertex->evolution->p, (float*)vertex->evolution->p, 1.0f - alpha, (float*)vertex->p);
			}
		}

		// Loop 2, create the shapes
		for (i = 0; i < geometry->edges.size; i++)
		{
			geom_edge_t *edge = (geom_edge_t*)geometry->edges.content[i];
			array_t edgeArray;
			geometry_t *otherGeometry = getGeometryOnOtherSideOfEdge(edge, geometry);
			geom_edge_t *edge1, *edge2, *edge3;

			if (!otherGeometry->faceVertex)
			{
				otherGeometry->faceVertex = newVertexFromPoint(newObject(point_t));
				*otherGeometry->faceVertex->p = otherGeometry->facePoint;
			}

			edge1 = newEdgeIfItDoesNotExistYet(geometry->faceVertex, edge->s1);
			edge2 = newEdgeIfItDoesNotExistYet(edge->s1, otherGeometry->faceVertex);
			edge3 = newEdgeIfItDoesNotExistYet(otherGeometry->faceVertex, geometry->faceVertex);

			//if (!edge1->geomRight || !edge2->geomRight || !edge3->geomRight)
			{
				Array.init(&edgeArray, 3);
				Array.add(&edgeArray, edge1);
				Array.add(&edgeArray, edge2);
				Array.add(&edgeArray, edge3);
				List.add(&newGeometries, newGeometryWithEdges(edgeArray));
			}

			edge1 = newEdgeIfItDoesNotExistYet(geometry->faceVertex, edge->s2);
			edge2 = newEdgeIfItDoesNotExistYet(edge->s2, otherGeometry->faceVertex);
			edge3 = newEdgeIfItDoesNotExistYet(otherGeometry->faceVertex, geometry->faceVertex);

			//if (!edge1->geomRight || !edge2->geomRight || !edge3->geomRight)
			{
				Array.init(&edgeArray, 3);
				Array.add(&edgeArray, edge1);
				Array.add(&edgeArray, edge2);
				Array.add(&edgeArray, edge3);
				List.add(&newGeometries, newGeometryWithEdges(edgeArray));
			}
		}

		geometryIterator = &(*geometryIterator)->next;
	}

	List.clear(geometries); // TODO: Free geometries
	*geometries = newGeometries;
}
