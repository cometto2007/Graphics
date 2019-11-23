#pragma once
#include "../../nclgl/SceneNode.h"
#include "Loader.h"

class Water :
	public SceneNode
{
public:
	Water(GLuint cubeMap);
	~Water();
	
	void Update(float msec) override;
	void Draw(const OGLRenderer& r) override;
private:
	Loader loader = Loader::getInstance();

	float waterRotate;
	GLuint cubeMap;
};

