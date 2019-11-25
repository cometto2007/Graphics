#pragma once
#include "../../nclgl/Camera.h"
#include "Landscape.h"
#include "Loader.h"

#define SHADOWSIZE 2048
#define LIGHT_X 23108
#define LIGHT_Y 11500
#define LIGHT_Z 15208

class Renderer : public OGLRenderer {
public:
	int post_passes = 10;

	void configureCameraPositions();
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void moveLight(float x, float y, float z);

	void toggleBlur() { isBlur = !isBlur; };
	void toggleSplitScreen() { isSplitScreen = !isSplitScreen; };
	void setPostPasses(int post_passes) { post_passes = post_passes; };

	static void setCameraConfsIndex(Camera* cam, int i) { 
		cam->setCameraIndex(i);
	};
	static void setCameraAuto(Camera* cam, bool b) {
		cam->setAutoCam(b);
	};

	void activateSlideshow(bool active);
	void setCameraPosFromIndex(int i) { camera->setCameraIndex(i); };
	
protected:
	Loader loader = Loader::getInstance();
	Landscape* root;
	
	Camera* camera;
	Camera* camera2;
	Mesh* rainDrop;
	Mesh* quadPost;
	Light* light;
	Light* light2;

	GLuint shadowTex;
	GLuint shadowFBO;
	GLuint bufferDepthTex;
	GLuint bufferColourTex[2];
	GLuint bufferFBO;
	GLuint processFBO;

	bool isSplitScreen;
	bool isBlur;

	void DrawRain();
	void DrawShadowScene();
	void DrawCombinedScene();
	void DrawSplitScreenScene();
	void PresentScene();
	void DrawPostProcess();

	void DrawNode(SceneNode* n, bool isShadow);

	void configureShadow();
	void configurePostProcessing();
};


