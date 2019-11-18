#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "Terrain.h"
#include "Tree.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void BindTextureToSamplerAndUniform(unsigned int textureUnit, GLuint tex, GLchar* uniformName, Shader* shader, GLuint texType);
	Camera* GetCamera() { return camera; };
	Terrain* GetTerrain() { return terrain; };

	void testMethod();
protected:
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* testShader;

	Tree* tree;

	Camera* camera;
	Terrain* terrain;
	Mesh* quad;
	Light* light;
	Light* light2;
	SceneNode* root;

	GLuint cubeMap;

	float waterRotate;

	void DrawWater();
	void DrawSkybox();
	void DrawTerrain();

	void DrawNode(SceneNode* n);
};


