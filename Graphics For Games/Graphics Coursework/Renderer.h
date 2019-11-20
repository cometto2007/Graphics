#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include <stdlib.h>
#include "Terrain.h"
#include "MousePicker.h"
#define SHADOWSIZE 2048

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void BindTextureToSamplerAndUniform(unsigned int textureUnit, GLuint tex, GLchar* uniformName, Shader* shader, GLuint texType);
	Camera* GetCamera() { return camera; };
	Terrain* GetTerrain() { return terrain; };

	void moveLight(float x, float z);

	void setMousePicker(MousePicker* mp) { this->mp = mp; };
	void setCameraConfsIndex(int i) { camera->setCameraIndex(i); };
	void setCameraAuto(bool b) { camera->setAutoCam(b); };
	
protected:
	MousePicker* mp;

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* testShader;
	Shader* shadowShader;
	Shader* sceneShader;
	Shader* test1Shader;

	Camera* camera;
	Terrain* terrain;
	Mesh* quad;
	OBJMesh* obj;
	SceneNode* cube;
	Light* light;
	Light* light2;
	SceneNode* root;

	GLuint cubeMap;
	GLuint shadowTex;
	GLuint shadowFBO;

	float waterRotate;

	void DrawWater();
	void DrawSkybox();
	void DrawTerrain();
	void DrawShadowScene();
	void DrawCombinedScene();

	void DrawNode(SceneNode* n);
	void configureScene();
	float RandomFloat(float a, float b);
};


