#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	//terrain = new Terrain(0.25f, 3000.0f, 1500.f);
	terrain = new Terrain(0.25f, 3000.0f, 3000.f);

	camera = new Camera({ -41.54, 0.58, Vector3(8034, 11979, 21759) });
	camera->addCameraConf({ -41.54, 0.58, Vector3(8034, 11979, 21759) });
	camera->addCameraConf({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });
	camera->addCameraConf({ -40, 270, Vector3(-2100, 3300, 2000) });

	//camera->addCameraConf2(Vector3(1295.94f, 6551.4f, 2252.96f), Vector3(8034, 11979, 21759) );
	//camera->addCameraConf2(Vector3(1295.94f, 6551.4f, 3000.0f), Vector3(1295.94f, 6551.4f, 2252.96f));
	//camera->addCameraConf2(Vector3(1295.94f, 6551.4f, 3000.0f), Vector3(-2100, 3300, 2000));

	/*camera2 = new Camera({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });
	camera2->addCameraConf({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });
	camera2->addCameraConf({ -40, 270, Vector3(-2100, 3300, 2000) });
	camera2->addCameraConf({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });*/

	quad = Mesh::GenerateQuad();
	quadPost = Mesh::GenerateQuad();

	rainDrop = Mesh::GenerateQuad();
	rainDrop->setTexture(loader.getWaterTex());

	isSplitScreen = false;
	isBlur = false;
	
	light = new Light(Vector3(LIGHT_X, LIGHT_Y, LIGHT_Z), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 0.05f);

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR "skyboxVertex.glsl", SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR "bumpVertex.glsl", SHADERDIR "bumpFragment.glsl");
	sceneNodeShadowShader = new Shader(SHADERDIR "sceneNodeShadowVertex.glsl", SHADERDIR "shadowFrag.glsl");
	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");
	sceneShader = new Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");
	testShader = new Shader(SHADERDIR "BVertex.glsl", SHADERDIR "BFragment.glsl");
	test1Shader = new Shader(SHADERDIR "terrainVertex.glsl", SHADERDIR "terrainFragment.glsl"); 
	calcShadowGrassField = new Shader(SHADERDIR "shadowGrassFieldVert.glsl", SHADERDIR "shadowGrassFieldFrag.glsl");
	
	postProcessShader = new Shader(SHADERDIR "t10_texturedVertex.glsl", SHADERDIR "t10_texturedFragment.glsl");
	processShader = new Shader(SHADERDIR "t10_texturedVertex.glsl", SHADERDIR "t10_processfrag.glsl");
	animationShader = new Shader(SHADERDIR"skeletonvertex.glsl", SHADERDIR"TexturedFragment.glsl");
	birdShadowShader = new Shader(SHADERDIR"birdShadowVertex.glsl", SHADERDIR"birdShadowFragment.glsl");

	if (!reflectShader->LinkProgram() ||
		!skyboxShader->LinkProgram() ||
		!lightShader->LinkProgram() ||
		!testShader->LinkProgram() ||
		!shadowShader->LinkProgram() ||
		!sceneShader->LinkProgram() ||
		!test1Shader->LinkProgram() ||
		!sceneNodeShadowShader->LinkProgram() ||
		!calcShadowGrassField->LinkProgram() ||
		!processShader->LinkProgram() ||
		!postProcessShader->LinkProgram() ||
		!animationShader->LinkProgram() ||
		!birdShadowShader->LinkProgram()) {
		return;
	}

	configureScene();
	configureShadow();
	configurePostProcessing();

	quad->setTexture(loader.getWaterTex());
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !quad->GetTexture()) {
		return;
	}
	waterRotate = 0.0f;
	projMatrix = Matrix4::Perspective(1.0f, 50000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete terrain;
	delete quad;
	delete quadPost;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;

	delete testShader;
	delete shadowShader;
	currentShader = 0;

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
	camera->moveCameraAuto(msec);
	//camera2->moveCameraAuto(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 5000.0f; 
	root->Update(msec);
}

void Renderer::DrawTerrain()
{
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	
	BindTextureToSamplerAndUniform(11, shadowTex, "shadowTex", currentShader, GL_TEXTURE_2D);
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	Matrix4 tempMatrix = shadowMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);
	terrain->Draw(*this);
}

void Renderer::DrawRain()
{
	float heightX = (RAW_WIDTH * HEIGHTMAP_X);
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z);

	for (size_t i = 0; i < 1000; i++) {
		modelMatrix = Matrix4::Translation(Vector3(RandomFloat(-heightX, heightX), RandomFloat(0, 10000.0f), RandomFloat(-heightZ, heightZ))) *
			Matrix4::Scale(Vector3(5, 30, 20));
		UpdateShaderMatrices();
		rainDrop->Draw();
	}
}

void Renderer::DrawWater()
{
	SetShaderLight(*light);
	BindTextureToSamplerAndUniform(0, loader.getGroundTex(), "diffuseTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(2, cubeMap, "cubeTex", currentShader, GL_TEXTURE_CUBE_MAP);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightY = 1550 * HEIGHTMAP_Y / 2.0f;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX*10, 1, heightZ*10)) * Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));
	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));


	UpdateShaderMatrices();
	Matrix4 tempMatrix = shadowMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

	quad->Draw();
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	UpdateShaderMatrices();
	quad->Draw();
	glDepthMask(GL_TRUE);
}

void Renderer::DrawNode(SceneNode* node, bool isShadow)
{
	if (node->GetMesh() || node->getObjMesh()) {
		if ((node->GetShader() && !isShadow) || (node->GetShadowShader() && isShadow)) {
			SetShaderLight(*light);
			if (!isShadow) SetCurrentShader(node->GetShader());
			else SetCurrentShader(node->GetShadowShader());
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
			modelMatrix = node->GetWorldTransform() * Matrix4::Scale(node->GetModelScale());
			UpdateShaderMatrices();

			Matrix4 tempMatrix = shadowMatrix * modelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);
			BindTextureToSamplerAndUniform(11, shadowTex, "shadowTex", GetCurrentShader(), GL_TEXTURE_2D);
			glUniform1f(glGetUniformLocation(GetCurrentShader()->GetProgram(), "height"), terrain->getHeight());
			
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

void Renderer::configureScene()
{
	root = new SceneNode();

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);
	root->SetTransform(Matrix4::Translation(Vector3(heightX, 0, heightZ)));
	root->SetShader(testShader);
	float scale = 80.0f;
	for (int i = 0; i < 100; i++) {
		Tree* tree = new Tree();
		tree->SetShader(testShader);
		tree->SetShadowShader(calcShadowGrassField);
		tree->SetModelScale(Vector3(scale, scale, scale));
		tree->SetTransform(Matrix4::Translation(Vector3(RandomFloat(-heightX, heightX), 0, RandomFloat(-heightZ, heightZ))));

		root->AddChild(tree);
	}

	scale = 0.1f;
	birds = new SceneNode();
	birds->SetTransform(Matrix4::Translation(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 2500.0f, 0)));
	birds->SetShader(testShader);
	for (int i = 0; i < 10; i++) {
		Bird* bird = new Bird(*loader.getBirdData(), RandomFloat(0.4f, 0.6f));
		bird->SetShader(animationShader);
		bird->SetShadowShader(birdShadowShader);
		bird->SetModelScale(Vector3(scale, scale, scale));
		bird->SetTransform(Matrix4::Rotation(-90, Vector3(1.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90, Vector3(0.0f, 0.0f, 1.0f)) * 
			Matrix4::Translation(Vector3(RandomFloat(-80, 80.0f), RandomFloat(0, 50.0f), RandomFloat(-200.0f, 200.0f))));
		birds->AddChild(bird);
	}

	root->AddChild(birds);
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

	SetCurrentShader(test1Shader);
	DrawTerrain();
	SetCurrentShader(shadowShader);
	DrawWater();
	DrawNode(root, true);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();
	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {
	SetCurrentShader(skyboxShader);
	DrawSkybox();
	SetCurrentShader(lightShader);
	DrawTerrain();
	SetCurrentShader(reflectShader);
	DrawWater();
	DrawNode(root, false);
	SetCurrentShader(postProcessShader);
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

	SetCurrentShader(processShader);
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
	SetCurrentShader(postProcessShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	quadPost->setTexture(bufferColourTex[0]);
	quadPost->Draw();
	glUseProgram(0);
}

float Renderer::RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
