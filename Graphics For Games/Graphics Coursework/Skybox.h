#pragma once
#include "../../nclgl/SceneNode.h"
#include "Loader.h"

class Skybox :
	public SceneNode
{
public:
	Skybox();


	void Draw(const OGLRenderer& r) override;

	GLuint cube_map() const
	{
		return cubeMap;
	}

private:
	GLuint cubeMap;
	Loader loader = Loader::getInstance();
};

