#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "Tree.h"
#include "Loader.h"
#include <stdlib.h>
#include "Terrain.h"
#define SHADOWSIZE 2048
#define CAMERA_X 23108
#define CAMERA_Y 11500
#define CAMERA_Z 15208

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	Camera* GetCamera() { return camera; };
	Terrain* GetTerrain() { return terrain; };

	void moveLight(float x, float y, float z);

	void setCameraConfsIndex(int i) { camera->setCameraIndex(i); };
	void setCameraAuto(bool b) { camera->setAutoCam(b); };
	
protected:
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* testShader;
	Shader* shadowShader;
	Shader* sceneShader;
	Shader* test1Shader;
	Shader* sceneNodeShadowShader;

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


