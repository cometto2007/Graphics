#include "Skybox.h"

Skybox::Skybox()
{
	cubeMap = loader.getSkyBox();
	mesh = Mesh::GenerateQuad();
	shader = loader.skybox_shader();
	shadowShader = nullptr;
}

Skybox::~Skybox()
{
	glDeleteTextures(1, &cubeMap);
}

void Skybox::Draw(const OGLRenderer& r)
{
	glDepthMask(GL_FALSE);
	SceneNode::Draw(r);
	glDepthMask(GL_TRUE);
}
