#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "Tree.h"
#include "Bird.h"
#include "Loader.h"
#include "Utility.h"
#include "Water.h"
#include <stdlib.h>
#include "Terrain.h"
#define SHADOWSIZE 2048
#define LIGHT_X 23108
#define LIGHT_Y 11500
#define LIGHT_Z 15208

class Renderer : public OGLRenderer {
public:
	int post_passes = 10;

	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void moveLight(float x, float y, float z);

	void toggleBlur() { isBlur = !isBlur; };
	void toggleSplitScreen() { isSplitScreen = !isSplitScreen; };
	void setPostPasses(int post_passes) { post_passes = post_passes; };

	void setCameraConfsIndex(int i) { 
		camera->setCameraIndex(i);
		//camera2->setCameraIndex(i);
	};
	void setCameraAuto(bool b) { 
		camera->setAutoCam(b);
		//camera2->setAutoCam(b);
	};
	
protected:
	Loader loader = Loader::getInstance();

	Camera* camera;
	Camera* camera2;
	Terrain* terrain;
	Mesh* quad;
	Mesh* rainDrop;
	Mesh* quadPost;
	SceneNode* cube;
	SceneNode* water;
	Light* light;
	Light* light2;
	SceneNode* root;
	SceneNode* birds;

	GLuint cubeMap;
	GLuint shadowTex;
	GLuint shadowFBO;
	GLuint bufferDepthTex;
	GLuint bufferColourTex[2];
	GLuint bufferFBO;
	GLuint processFBO;

	bool isSplitScreen;
	bool isBlur;

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

	void DrawNode(SceneNode* n, bool isShadow);

	void configureScene();
	void configureShadow();
	void configurePostProcessing();
};


