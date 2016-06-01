#pragma once

#include <utils_ctools.h>
#include <utils_list.h>
#include <utils_array.h>
#include <utils_vector.h>
#include <utils_math.h>

typedef struct {
	float x, y, z;
} point_t;

#define point2Vec(p, v) v[0] = p->x; v[1] = p->y; v[2] = p->z;

typedef struct geom_vertex_s {
	point_t		*p;

	list_t		*edges;
	list_t		*geometries;

	struct geom_vertex_s	*evolution;
} geom_vertex_t;

typedef struct {
	geom_vertex_t	*s1;
	geom_vertex_t	*s2;

	void		*geomLeft;
	void		*geomRight;

	// Precalculated data
	float		direction[3];
	float		left[3];
	float		right[3];
	point_t		edgePoint;

	geom_vertex_t	*edgeVertex;
} geom_edge_t;

typedef struct {
	array_t		edges;

	float		color[3];

	// Precalculated data
	point_t		facePoint;
	array_t		uniqueVertices;
} geometry_t;

#define GEOM_LEFT(edge) ((geometry_t*)edge->geomLeft)
#define GEOM_RIGHT(edge) ((geometry_t*)edge->geomRight)

list_t *geometriesForCube(void);

geom_vertex_t *newVertexFromPoint(point_t *point);
geom_edge_t *newEdgeWithVertices(geom_vertex_t *v1, geom_vertex_t *v2);
geom_edge_t *newEdgeIfItDoesNotExistYet(geom_vertex_t *v1, geom_vertex_t *v2);
geometry_t *newGeometryWithEdges(array_t edges);
void getEdgesOfGeometryThatGoThroughVertex(const geometry_t *geometry, const geom_vertex_t *vertex, geom_edge_t **edge1, geom_edge_t **edge2);
