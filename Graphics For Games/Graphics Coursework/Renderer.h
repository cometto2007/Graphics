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
#define LIGHT_X 23108
#define LIGHT_Y 11500
#define LIGHT_Z 15208

#define POST_PASSES 0

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	Camera* GetCamera() { return camera; };
	Terrain* GetTerrain() { return terrain; };

	void moveLight(float x, float y, float z);

	void setCameraConfsIndex(int i) { 
		camera->setCameraIndex(i);
		camera2->setCameraIndex(i);
	};
	void setCameraAuto(bool b) { 
		camera->setAutoCam(b);
		camera2->setAutoCam(b);
	};
	
protected:
	Loader loader = Loader::getInstance();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* testShader;
	Shader* shadowShader;
	Shader* sceneShader;
	Shader* test1Shader;
	Shader* sceneNodeShadowShader;
	Shader* calcShadowGrassField;
	Shader* processShader;
	Shader* postProcessShader;

	Camera* camera;
	Camera* camera2;
	Terrain* terrain;
	Mesh* quad;
	Mesh* rainDrop;
	Mesh* quadPost;
	OBJMesh* obj;
	SceneNode* cube;
	Light* light;
	Light* light2;
	SceneNode* root;

	GLuint cubeMap;
	GLuint shadowTex;
	GLuint shadowFBO;
	GLuint bufferDepthTex;
	GLuint bufferColourTex[2];
	GLuint bufferFBO;
	GLuint processFBO;

	float waterRotate;

	void DrawWater();
	void DrawSkybox();
	void DrawTerrain();
	void DrawRain();
	void DrawShadowScene();
	void DrawCombinedScene();
	void DrawSplitScreenScene();
	void PresentScene();
	void DrawPostProcess();

	void DrawNode(SceneNode* n);

	void configureScene();
	void configureShadow();
	void configurePostProcessing();

	float RandomFloat(float a, float b);
};


