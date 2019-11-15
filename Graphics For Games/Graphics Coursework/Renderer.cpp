#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	terrain = new Terrain();
	camera = new Camera(-40, 270, Vector3(-2100, 3300, 2000));

	currentShader = new Shader(SHADERDIR"TexturedVertex.glsl",
		SHADERDIR "TexturedFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}

	terrain->setTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	terrain->setDeptText(SOIL_load_OGL_texture(TEXTUREDIR"terrain.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	terrain->setWaterText(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!terrain->getDeptText() || !terrain->GetTexture() || !terrain->getWaterText()) {
		return;
	}
	SetTextureRepeating(terrain->GetTexture(), true);
	SetTextureRepeating(terrain->getDeptText(), true);
	SetTextureRepeating(terrain->getWaterText(), true);


	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete terrain;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	BindTextureToSamplerAndUniform(0, terrain->GetTexture(), "diffuseTex", currentShader);
	BindTextureToSamplerAndUniform(1, terrain->getDeptText(), "deptTex", currentShader);
	BindTextureToSamplerAndUniform(2, terrain->getWaterText(), "waterTex", currentShader);
	terrain->Draw();

	glUseProgram(0);
	SwapBuffers();
}

void Renderer::BindTextureToSamplerAndUniform(unsigned int textureUnit, GLuint tex, GLchar* uniformName, Shader* shader)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), uniformName), textureUnit);
}
