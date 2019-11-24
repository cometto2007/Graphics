#include "Renderer.h"
#include "Utility.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	root = new Landscape();
	camera = new Camera({ -7.86, 355.75, Vector3(5344, 8476, 74991) });

	quadPost = Mesh::GenerateQuad();
	rainDrop = Mesh::GenerateQuad();
	rainDrop->setTexture(loader.getWaterTex());

	isSplitScreen = false;
	isBlur = false;
	
	light = new Light(Vector3(LIGHT_X, LIGHT_Y, LIGHT_Z), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 0.05f);

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
	delete quadPost;
	delete light;
	currentShader = nullptr;

	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
	SwapBuffers();
}

void Renderer::UpdateScene(float msec) {
	// TODO: add if
	camera->UpdateCamera(msec);
	//camera2->UpdateCamera(msec);
	if (camera->getAutoCam()) {
		CameraEffects effects = camera->moveCameraAuto(msec);
		if (effects.isBlur != this->isBlur) {
			this->toggleBlur();
		}
		if (effects.isSplitScreen != this->isSplitScreen) {
			this->toggleSplitScreen();
		}
	}
	//camera2->moveCameraAuto(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer::DrawRain()
{
	float heightX = (RAW_WIDTH * HEIGHTMAP_X);
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z);

	for (size_t i = 0; i < 1000; i++) {
		modelMatrix = Matrix4::Translation(Vector3(Utility::RandomFloat(-heightX, heightX), Utility::RandomFloat(0, 10000.0f), Utility::RandomFloat(-heightZ, heightZ))) *
			Matrix4::Scale(Vector3(5, 30, 20));
		UpdateShaderMatrices();
		rainDrop->Draw();
	}
}

void Renderer::DrawNode(SceneNode* node, bool isShadow)
{
	if (node->GetMesh() || node->getObjMesh()) {
		if ((node->GetShader() && !isShadow) || (node->GetShadowShader() && isShadow)) {
			if (!isShadow) SetCurrentShader(node->GetShader());
			else SetCurrentShader(node->GetShadowShader());
			SetShaderLight(*light);
			
			
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
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
	for (vector<SceneNode*>::const_iterator i = node->getChildIteratorStart(); i != node->getChildIteratorEnd(); ++i) {
		DrawNode(*i, isShadow);
	}
}

// TODO: delete
void Renderer::moveLight(float x, float y, float z)
{
	Vector3 pos = light->GetPosition();
	light->SetPosition({pos.x + x, pos.y + y, pos.z + z});
	cout << light->GetPosition() << endl;
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
	DrawNode(root, true);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();
	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {
	DrawNode(root, false);
	SetCurrentShader(loader.post_process_shader());
	DrawRain();
	glUseProgram(0);
}

void Renderer::DrawSplitScreenScene()
{
	//viewMatrix = camera2->BuildViewMatrix();
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

void Renderer::configureCameraPositions()
{
	camera->addCameraConf({ { -15.77, 5, Vector3(5376, 8476, 77322) }, {0.5, false, false} });
	camera->addCameraConf({ { -7.86, 355.75, Vector3(5344, 8476, 74991) }, {40, true, false} });
	camera->addCameraConf({ { -38.94, 357.57, Vector3(8073, 8476, 19475) }, {1, false, false} });
	camera->addCameraConf({ { -47.76, 65.96, Vector3(16887, 8476, 13162) }, {1, false, false} });
	camera->addCameraConf({ { -40.27, 128.75, Vector3(17825, 8476, 2022) }, {1, false, false} });
	camera->addCameraConf({ { -35.79, 210.58, Vector3(547, 8476, -2025) }, {1, false, false} });
	camera->addCameraConf({ { -43.00, 305.57, Vector3(-1588, 8476, 14765) }, {1, false, false} });
	camera->addCameraConf({ { -41.54, 0.58, Vector3(8034, 11979, 21759) }, {1, false, false} });

	//camera->addCameraConf2(Vector3(1295.94f, 6551.4f, 2252.96f), Vector3(8034, 11979, 21759) );
	//camera->addCameraConf2(Vector3(1295.94f, 6551.4f, 3000.0f), Vector3(1295.94f, 6551.4f, 2252.96f));
	//camera->addCameraConf2(Vector3(1295.94f, 6551.4f, 3000.0f), Vector3(-2100, 3300, 2000));

	/*camera2 = new Camera({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });
	camera2->addCameraConf({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });
	camera2->addCameraConf({ -40, 270, Vector3(-2100, 3300, 2000) });
	camera2->addCameraConf({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });*/
}
