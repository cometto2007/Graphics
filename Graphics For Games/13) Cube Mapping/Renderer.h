#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"

#define POST_PASSES 10

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	Camera* camera;
	HeightMap* heightMap;
	Mesh* quad;
	Light* light;

	GLuint cubeMap;

	float waterRotate;
};
