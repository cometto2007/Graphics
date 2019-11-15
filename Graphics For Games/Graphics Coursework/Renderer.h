#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "Terrain.h"

class Renderer :
	public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void BindTextureToSamplerAndUniform(unsigned int textureUnit, GLuint tex, GLchar* uniformName, Shader* shader);
protected:
	Camera* camera;
	Terrain* terrain;
};


