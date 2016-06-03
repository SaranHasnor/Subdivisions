#include <stdio.h>
#include <engine.h>
#include <engine_utils.h>

#include <utils_init.h>
#include <engine_init.h>

#include "mesh.h"
#include "subdivision.h"

list_t *_geometries = NULL;
mesh_t *_mesh = NULL;

static float w = 0.0f;
static float oldW = 0.0f;
static float *wPtr = NULL;

void keyDownFunc(uchar key)
{
	if (key == 27)
	{
		Engine.shutdown();
	}
	
	if (_geometries)
	{
		bool regenerate = false;

		if (key == '1')
		{
			subdivideCatmull(&_geometries);
			regenerate = true;
		}
		else if (key == '2')
		{
			subdivideLoop(&_geometries);
			regenerate = true;
		}
		else if (key == '3')
		{
			subdivideKobbelt(&_geometries);
			regenerate = true;
		}
		else if (key == '4')
		{
			subdivideButterfly(&_geometries, w);
			regenerate = true;
		}
		else if (key == 't' || key == 'r')
		{
			List.clear(&_geometries);
			_geometries = geometriesForPyramid();
			regenerate = true;
		}
		else if (key == 'c')
		{
			List.clear(&_geometries);
			_geometries = geometriesForCube();
			regenerate = true;
		}

		if (regenerate)
		{
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
	uint id;
	_geometries = geometriesForPyramid();
	_mesh = meshWithGeometry(_geometries);

	id = Engine.UI.slider(staticPlacement(20, 20, 200, 40));
	Engine.UI.setSliderValue(id, -1.0f / 16.0f, 1.0f / 16.0f, &w);
	id = Engine.UI.textField(staticPlacement(240, 20, 100, 40), FIELDTYPE_FLOAT);
	wPtr = newObject(float);
	*wPtr = w;
	Engine.UI.setTextFieldValue(id, -1.0f, 1.0f, (void**)&wPtr, false);
}

void updateFunc(const timeStruct_t time, const inputStruct_t input)
{
	updateCamera(input);

	if (w != oldW)
	{
		oldW = w;
		*wPtr = w;
		_geometries = geometriesForPyramid();
		subdivideButterfly(&_geometries, w);
		subdivideButterfly(&_geometries, w);
		if (_mesh) Mesh.destroyMesh(_mesh);
		_mesh = meshWithGeometry(_geometries);
	}
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