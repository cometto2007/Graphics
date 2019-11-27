#include "Renderer.h"
#include "Utility.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	root = new Landscape();
	camera = new Camera({ -38.94, 357.57, Vector3(8073, 8476, 19475) });

	quadPost = Mesh::GenerateQuad();
	rain = new Rain();

	isSplitScreen = false;
	isBlur = false;
	
	light = new Light(Vector3(23608, 11500, 15208), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 0.05f);
	light2 = new Light(Vector3(-13792, 11500, -4092), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 0.5f);

	configureShadow();
	configurePostProcessing();
	configureCameraPositions();

	projMatrix = Matrix4::Perspective(1.0f, 50000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete camera2;
	delete root;
	delete quadPost;
	delete light;
	delete light2;
	
	currentShader = nullptr;

	glDeleteTextures(1, &shadowTex);
	glDeleteTextures(1, &shadowFBO);

	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	BuildNodeLists(root);

	DrawShadowScene();
	if (isBlur) {
		glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		if (isSplitScreen) DrawSplitScreenScene();
		else DrawCombinedScene();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		DrawPostProcess();
		PresentScene();
	} else {
		if (isSplitScreen) DrawSplitScreenScene();
		else DrawCombinedScene();
	}
	transparentNodeList.clear();
	nodeList.clear();
	SwapBuffers();
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	if (camera->getAutoCam()) {
		CameraEffects effects = camera->moveCameraAuto(msec);
		if (effects.isBlur != this->isBlur) {
			this->toggleBlur();
		}
		if (effects.isSplitScreen != this->isSplitScreen) {
			this->toggleSplitScreen();
			if (effects.isSplitScreen) {
				setCameraAuto(camera2, true);
			} else {
				setCameraAuto(camera2, false);
				setCameraConfsIndex(camera2, 0);
			}
		}
		if (effects.isSplitScreen) {
			camera2->moveCameraAuto(msec);
		}
	}
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	
	root->Update(msec);
	rain->Update(msec);
}

void Renderer::DrawNode(SceneNode* node, bool isShadow)
{
	if (node->GetMesh() || node->getObjMesh()) {
		if ((node->GetShader() && !isShadow) || (node->GetShadowShader() && isShadow)) {
			if (!isShadow) SetCurrentShader(node->GetShader());
			else SetCurrentShader(node->GetShadowShader());
			SetShaderLight(*light);
			SetShaderLights2(*light, *light2);
			
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

			for (int i = 0; i < node->getDrawingMesh(); i++) {
				node->updateDrawing(i);
				modelMatrix = node->GetWorldTransform() * Matrix4::Scale(node->GetModelScale());
				textureMatrix = node->texture_matrix();
				UpdateShaderMatrices();

				Matrix4 tempMatrix = shadowMatrix * modelMatrix;
				glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);
				BindTextureToSamplerAndUniform(11, shadowTex, "shadowTex", GetCurrentShader(), GL_TEXTURE_2D);
				glUniform1f(glGetUniformLocation(GetCurrentShader()->GetProgram(), "height"), root->get_terrain()->getHeight());

				node->Draw(*this);
			}
			
		}
	}
}

void Renderer::activateSlideshow(bool active)
{
	if (!active) {
		isBlur = false;
		isSplitScreen = false;
	} else {
		camera->setCameraIndex(0);
	}
	camera->setAutoCam(active);
	
}

void Renderer::configurePostProcessing()
{
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &processFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::configureShadow()
{
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	projMatrix = Matrix4::Perspective(10.0f, 100000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	shadowMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();
	SetCurrentShader(loader.shadow_shader());
	DrawNodes(true);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();
	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {
	DrawNodes(false);
	SetCurrentShader(loader.rain_shader());
	DrawNode(rain, false);
	glUseProgram(0);
}

void Renderer::DrawSplitScreenScene()
{
	viewMatrix = camera2->BuildViewMatrix();
	glViewport(0, 0, width, height / 2);
	DrawCombinedScene();

	viewMatrix = camera->BuildViewMatrix();
	glViewport(0, height / 2, width, height / 2);
	DrawCombinedScene();
}

void Renderer::DrawPostProcess()
{
	glViewport(0, 0, width, height);
	
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(loader.process_shader());
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, 1, -1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	glDisable(GL_DEPTH_TEST);
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	for (int i = 0; i < post_passes; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);
		quadPost->setTexture(bufferColourTex[0]);
		quadPost->Draw();
		
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
		quadPost->setTexture(bufferColourTex[1]);
		quadPost->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::PresentScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetCurrentShader(loader.post_process_shader());
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	quadPost->setTexture(bufferColourTex[0]);
	quadPost->Draw();
	glUseProgram(0);
}

void Renderer::BuildNodeLists(SceneNode* from)
{
	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->getIsTransparent()) {
			transparentNodeList.push_back(from);
		} else {
			nodeList.push_back(from);
		}
	}
	for (vector<SceneNode*>::const_iterator i = from->getChildIteratorStart(); i != from->getChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists()
{
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes(bool isShadow)
{
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i), isShadow);
	}
	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i), isShadow);
	}
}

void Renderer::configureCameraPositions()
{
	camera->addCameraConf({ { -15.77, 5, Vector3(5376, 8476, 77322) }, {0.2, false, false} });
	camera->addCameraConf({ { -7.86, 355.75, Vector3(5344, 8476, 74991) }, {40, true, false} });
	camera->addCameraConf({ { -38.94, 357.57, Vector3(8073, 8476, 19475) }, {1, false, false} });
	camera->addCameraConf({ { -47.76, 65.96, Vector3(16887, 8476, 13162) }, {1, false, true} });
	camera->addCameraConf({ { -40.27, 128.75, Vector3(17825, 8476, 2022) }, {1, false, true} });
	camera->addCameraConf({ { -35.79, 210.58, Vector3(547, 8476, -2025) }, {1, false, true} });
	camera->addCameraConf({ { -43.00, 305.57, Vector3(-1588, 8476, 14765) }, {1, false, false} });
	camera->addCameraConf({ { -41.54, 0.58, Vector3(8034, 11979, 21759) }, {1, false, false} });

	camera2 = new Camera({ -17.80f, 318.86f, Vector3(874, 1961, 14681) });
	camera2->addCameraConf({ { -17.80f, 318.86f, Vector3(874, 1961, 14681) }, {1, false, false} });
	camera2->addCameraConf({ { -17.24f, 317.95f, Vector3(8136, 1961, 6629) }, {1, false, false} });
	camera2->addCameraConf({ { -9.61, 201.06, Vector3(5214, 1961, 3295) }, {1, false, false} });
	camera2->addCameraConf({ { -10.10, 264.62, Vector3(1941, 1961, 10084) }, {1, false, false} });
	camera2->addCameraConf({ { -5.34, 358.91, Vector3(7579, 1961, 14455) }, {1, false, false} });
	camera2->addCameraConf({ { -17.80f, 318.86f, Vector3(874, 1961, 14681) }, {1, false, false} });
	camera2->addCameraConf({ { -17.24f, 317.95f, Vector3(8136, 1961, 6629) }, {1, false, false} });
	camera2->addCameraConf({ { -9.61, 201.06, Vector3(5214, 1961, 3295) }, {1, false, false} });
	camera2->addCameraConf({ { -10.10, 264.62, Vector3(1941, 1961, 10084) }, {1, false, false} });
	camera2->addCameraConf({ { -5.34, 358.91, Vector3(7579, 1961, 14455) }, {1, false, false} });
	camera2->addCameraConf({ { -17.80f, 318.86f, Vector3(874, 1961, 14681) }, {1, false, false} });
	camera2->addCameraConf({ { -17.24f, 317.95f, Vector3(8136, 1961, 6629) }, {1, false, false} });
	camera2->addCameraConf({ { -9.61, 201.06, Vector3(5214, 1961, 3295) }, {1, false, false} });
	camera2->addCameraConf({ { -10.10, 264.62, Vector3(1941, 1961, 10084) }, {1, false, false} });
	camera2->addCameraConf({ { -5.34, 358.91, Vector3(7579, 1961, 14455) }, {1, false, false} });
}
