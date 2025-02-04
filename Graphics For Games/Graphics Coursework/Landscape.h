#pragma once
#include "../../nclgl/SceneNode.h"
#include "Loader.h"
#include "Terrain.h"


class Landscape :
	public SceneNode
{
public:
	Landscape();

	Terrain* get_terrain() { return terrain; };

private:
	Loader loader = Loader::getInstance();

	SceneNode* birds;
	Terrain* terrain;

	GLuint cubeMap;
};

