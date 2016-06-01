#include "mesh.h"

mesh_t *cubeWithQuadFaces(void)
{
	mesh_t *mesh = Mesh.newMesh();
	face_t *face1, *face2;
	
	// Bottom
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f);

	Vector.set(face1->color, 1.0f, 0.0f, 0.0f);
	Vector.copy(face2->color, face1->color);

	// Top
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

	Vector.set(face1->color, 1.0f, 0.0f, 0.0f);
	Vector.copy(face2->color, face1->color);

	// Left
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 1.0f, 0.0f);
	Vector.copy(face2->color, face1->color);

	// Right
	face1 = Mesh.addFace();
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f);

	Vector.set(face1->color, 1.0f, 1.0f, 0.0f);
	Vector.copy(face2->color, face1->color);

	// Front
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 0.0f, 1.0f);
	Vector.copy(face2->color, face1->color);

	// Back
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 0.0f, 1.0f);
	Vector.copy(face2->color, face1->color);
	
	Mesh.updateGeometry(mesh);

	return mesh;
}

mesh_t *cubeWithTriFaces(void)
{
	mesh_t *mesh = Mesh.newMesh();
	face_t *face1, *face2;
	
	// Bottom
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f);

	Vector.set(face1->color, 1.0f, 0.0f, 0.0f);
	Vector.set(face2->color, 1.0f, 1.0f, 0.0f);

	// Top
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 0.0f, 1.0f);
	Vector.set(face2->color, 1.0f, 0.0f, 0.0f);

	// Left
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 0.0f, 1.0f);
	Vector.set(face2->color, 1.0f, 1.0f, 0.0f);

	// Right
	face1 = Mesh.addFace();
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 1.0f, 0.0f);
	Vector.set(face2->color, 1.0f, 1.0f, 0.0f);

	// Front
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
	Mesh.addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 0.0f, 1.0f);
	Vector.set(face2->color, 0.0f, 1.0f, 0.0f);

	// Back
	face1 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	face2 = Mesh.addFace();
	Mesh.addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	Mesh.addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	Mesh.addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);

	Vector.set(face1->color, 0.0f, 1.0f, 0.0f);
	Vector.set(face2->color, 1.0f, 0.0f, 0.0f);
	
	Mesh.updateGeometry(mesh);

	return mesh;
}

face_t *_faceWithGeometry(const geometry_t *geometry)
{
	face_t *newFace = Mesh.addFace();
	uint i;

	for (i = 0; i < geometry->uniqueVertices.size; i++)
	{
		geom_vertex_t *vert = (geom_vertex_t*)geometry->uniqueVertices.content[i];
		Mesh.addVertex(vert->p->x, vert->p->y, vert->p->z, 0.0f, 0.0f);
	}

	Vector.copy(newFace->color, geometry->color);

	return newFace;
}

mesh_t *meshWithGeometry(list_t *geometries)
{
	mesh_t *newMesh = Mesh.newMesh();
	list_t **geometryIterator = &geometries;

	while (*geometryIterator)
	{
		geometry_t *geom = (geometry_t*)(*geometryIterator)->content;
		face_t *face = _faceWithGeometry(geom);
		//Mesh.linkFaceToMesh(newMesh, face);

		geometryIterator = &(*geometryIterator)->next;
	}

	Mesh.updateGeometry(newMesh);

	return newMesh;
}
