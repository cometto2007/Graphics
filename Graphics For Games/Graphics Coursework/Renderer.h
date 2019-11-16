#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "Terrain.h"

class Renderer :
	public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void BindTextureToSamplerAndUniform(unsigned int textureUnit, GLuint tex, GLchar* uniformName, Shader* shader);
	Camera* GetCamera() { return camera; };
	Terrain* GetTerrain() { return terrain; };
protected:
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	OBJMesh* test;

	Camera* camera;
	Terrain* terrain;
	Mesh* quad;
	Light* light;

	GLuint cubeMap;

	float waterRotate;

	void DrawWater();
	void DrawSkybox();
	void DrawTerrain();
};


