#pragma once
#include "../../nclgl/SceneNode.h"
#include "Loader.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Tree.h"

class Scenary :
	public SceneNode
{
public:
	Scenary();

private:
	Loader loader = Loader::getInstance();

	SceneNode* birds;
	Terrain* terrain;

	GLuint cubeMap;
};

