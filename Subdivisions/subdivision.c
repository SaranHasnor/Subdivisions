#include "subdivision.h"

void subdivideGeometry(list_t **geometries)
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

					R.x += edge->edgePoint.x;
					R.y += edge->edgePoint.y;
					R.z += edge->edgePoint.z;

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
				*edge1->edgeVertex->p = edge1->edgePoint;
			}
			if (!edge2->edgeVertex)
			{
				edge2->edgeVertex = newVertexFromPoint(newObject(point_t));
				*edge2->edgeVertex->p = edge2->edgePoint;
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