#include <stdio.h>
#include <engine.h>
#include <engine_utils.h>

#include <utils_init.h>
#include <engine_init.h>

#include "mesh.h"
#include "subdivision.h"

list_t *_geometries = NULL;
mesh_t *_mesh = NULL;

void keyDownFunc(uchar key)
{
	if (key == 27)
	{
		Engine.shutdown();
	}
	else if (key == 'b')
	{
		if (_geometries)
		{
			subdivideGeometry(&_geometries);
			if (_mesh) Mesh.destroyMesh(_mesh);
			_mesh = meshWithGeometry(_geometries);
		}
	}
}

void keyUpFunc(uchar key)
{

}

void mouseDownFunc(ushort button, int x, int y)
{

}

void mouseUpFunc(ushort button, int x, int y)
{

}

void updateCamera(inputStruct_t input)
{
	float velocity[3] = {0,0,0};

	if (input.mouseButtons & INPUT_MOUSELEFT)
	{
		float angle[3];

		angle[0] = (float)input.mouseDelta[1];
		angle[1] = -(float)input.mouseDelta[0];
		angle[2] = 0;

		Engine.Camera.rotate(angle);
	}

	if (input.key_timeHeld('z') > 0)
	{
		velocity[1] += 1.0f;
	}

	if (input.key_timeHeld('s') > 0)
	{
		velocity[1] -= 1.0f;
	}

	if (input.key_timeHeld('d') > 0)
	{
		velocity[0] += 1.0f;
	}

	if (input.key_timeHeld('q') > 0)
	{
		velocity[0] -= 1.0f;
	}

	if (input.key_timeHeld(' ') > 0)
	{
		velocity[2] += 1.0f;
	}

	if (input.key_timeHeld('x') > 0)
	{
		velocity[2] -= 1.0f;
	}

	Vector.scale(velocity, 2.0f, velocity);

	Engine.Camera.setVelocity(velocity);
}

void initFunc(void)
{
	_mesh = cubeWithTriFaces();

	_geometries = geometriesForCube();
	_mesh = meshWithGeometry(_geometries);
}

void updateFunc(const timeStruct_t time, const inputStruct_t input)
{
	updateCamera(input);
}

void renderFunc(const float viewMatrix[16])
{
	Mesh.render(_mesh, viewMatrix);
}

int main(int argc, char **argv)
{
	engineListener_t listener;

	listener.keyDownFunc = keyDownFunc;
	listener.keyUpFunc = keyUpFunc;
	listener.mouseDownFunc = mouseDownFunc;
	listener.mouseUpFunc = mouseUpFunc;
	listener.renderFunc = renderFunc;
	listener.updateFunc = updateFunc;
	listener.initFunc = initFunc;

	initHasnorUtils();
	initHasnorEngine();

	Engine.run(argc, argv, 1200, 600, "Subdivisions", listener);

	return 0;
}