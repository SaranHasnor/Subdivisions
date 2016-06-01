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

	if (geometry->uniqueVertices.size == 4)
	{ // Special case for quads, make sure they're drawn in the right order
		geom_vertex_t **vertices = (geom_vertex_t**)geometry->uniqueVertices.content;
		float diff1[3], diff2[3], diff3[3];
		float angle1, angle2, angle3;

		Vector.subtract(diff1, (float*)vertices[1]->p, (float*)vertices[0]->p);
		Vector.subtract(diff2, (float*)vertices[2]->p, (float*)vertices[0]->p);
		Vector.subtract(diff3, (float*)vertices[3]->p, (float*)vertices[0]->p);

		angle1 = Vector.angleBetween(diff1, diff2);
		angle2 = Vector.angleBetween(diff2, diff3);
		angle3 = Vector.angleBetween(diff3, diff1);

		if (angle3 >= angle1 && angle3 >= angle2)
		{
			Mesh.addVertex(vertices[0]->p->x, vertices[0]->p->y, vertices[0]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[1]->p->x, vertices[1]->p->y, vertices[1]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[2]->p->x, vertices[2]->p->y, vertices[2]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[3]->p->x, vertices[3]->p->y, vertices[3]->p->z, 0.0f, 0.0f);
		}
		else if (angle1 >= angle2 && angle1 >= angle3)
		{
			Mesh.addVertex(vertices[0]->p->x, vertices[0]->p->y, vertices[0]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[1]->p->x, vertices[1]->p->y, vertices[1]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[3]->p->x, vertices[3]->p->y, vertices[3]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[2]->p->x, vertices[2]->p->y, vertices[2]->p->z, 0.0f, 0.0f);
		}
		else if (angle2 >= angle1 && angle2 >= angle3)
		{
			Mesh.addVertex(vertices[0]->p->x, vertices[0]->p->y, vertices[0]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[2]->p->x, vertices[2]->p->y, vertices[2]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[1]->p->x, vertices[1]->p->y, vertices[1]->p->z, 0.0f, 0.0f);
			Mesh.addVertex(vertices[3]->p->x, vertices[3]->p->y, vertices[3]->p->z, 0.0f, 0.0f);
		}
	}
	else
	{
		for (i = 0; i < geometry->uniqueVertices.size; i++)
		{
			geom_vertex_t *vert = (geom_vertex_t*)geometry->uniqueVertices.content[i];
			Mesh.addVertex(vert->p->x, vert->p->y, vert->p->z, 0.0f, 0.0f);
		}
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
