#pragma once
#include "../../nclgl/Frustum.h"
#include "../../nclgl/Camera.h"
#include "Landscape.h"
#include "Loader.h"

#include <algorithm>
#include "Rain.h"

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer {
public:
	int post_passes = 10;

	void configureCameraPositions();
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

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
	void setCameraPosFromIndex(int i)
	{
		camera->setCameraIndex(i);
		camera2->setCameraIndex(0);
	};
	
protected:
	Loader loader = Loader::getInstance();
	Landscape* root;
	
	Camera* camera;
	Camera* camera2;
	Mesh* quadPost;
	Light* light;
	Light* light2;

	Rain* rain;

	GLuint shadowTex;
	GLuint shadowFBO;
	GLuint bufferDepthTex;
	GLuint bufferColourTex[2];
	GLuint bufferFBO;
	GLuint processFBO;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	bool isSplitScreen;
	bool isBlur;

	void DrawShadowScene();
	void DrawCombinedScene();
	void DrawSplitScreenScene();
	void PresentScene();
	void DrawPostProcess();
	void DrawNode(SceneNode* n, bool isShadow);

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes(bool isShadow);

	void configureShadow();
	void configurePostProcessing();
};


