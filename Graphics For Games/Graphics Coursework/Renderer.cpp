#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	terrain = new Terrain(2.0f, 3000.0f, 3000.f);

	camera = new Camera({ -40, 270, Vector3(-2100, 3300, 2000) });
	camera->addCameraConf({ -40, 270, Vector3(-2100, 3300, 2000) });
	camera->addCameraConf({ -83.0499f, 270.07f, Vector3(1295.94f, 6551.4f, 2252.96f) });
	camera->addCameraConf({ -40, 270, Vector3(-2100, 3300, 2000) });

	quad = Mesh::GenerateQuad();
	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 1.0f);

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR "skyboxVertex.glsl", SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR "bumpVertex.glsl", SHADERDIR "bumpFragment.glsl");
	testShader = new Shader(SHADERDIR "BVertex.glsl", SHADERDIR "BFragment.glsl");

	if (!reflectShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram() || !testShader->LinkProgram()) {
		return;
	}

	configureScene();
	quad->setTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !quad->GetTexture()) {
		return;
	}
	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(terrain->GetTexture(), true);
	SetTextureRepeating(terrain->GetBumpMap(), true);
	SetTextureRepeating(terrain->getGrassText(), true);
	SetTextureRepeating(terrain->getCliffsText(), true);
	SetTextureRepeating(terrain->getSandText(), true);
	SetTextureRepeating(terrain->getSandText(), true);

	waterRotate = 0.0f;
	projMatrix = Matrix4::Perspective(1.0f, 10000000.0f, (float)width / (float)height, 45.0f);

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
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	currentShader = 0;
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawTerrain();
	DrawWater();
	
	testMethod();

	SwapBuffers();
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	camera->moveCameraAuto(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 1000.0f; 
	root->Update(msec);
}

void Renderer::DrawTerrain()
{
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	terrain->updateHeight();
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "height"), terrain->getHeight());
	BindTextureToSamplerAndUniform(0, terrain->GetTexture(), "diffuseTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(1, terrain->GetBumpMap(), "bumpTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(2, terrain->getDeptMap(), "deptTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(3, terrain->getGrassMap(), "grassMap", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(4, terrain->getSandMap(), "sandMap", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(5, terrain->getSandWetMap(), "sandWetMap", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(6, terrain->getCliffsMap(), "cliffsMap", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(7, terrain->getGrassText(), "grassTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(8, terrain->getSandText(), "sandTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(9, terrain->getSandWetText(), "sandWetTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(10, terrain->getCliffsText(), "cliffsTex", currentShader, GL_TEXTURE_2D);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	terrain->Draw();

	glUseProgram(0);
}

void Renderer::DrawWater()
{
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);
	BindTextureToSamplerAndUniform(0, terrain->GetTexture(), "diffuseTex", currentShader, GL_TEXTURE_2D);
	BindTextureToSamplerAndUniform(2, cubeMap, "cubeTex", currentShader, GL_TEXTURE_CUBE_MAP);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightY = 1550 * HEIGHTMAP_Y / 2.0f;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) * Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));
	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::BindTextureToSamplerAndUniform(unsigned int textureUnit, GLuint tex, GLchar* uniformName, Shader* shader, GLuint texType)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(texType, tex);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), uniformName), textureUnit);
}

void Renderer::testMethod() {
	DrawNode(root);
}

void Renderer::DrawNode(SceneNode* node)
{
	glUseProgram(currentShader->GetProgram());
	if (node->GetMesh() || node->getObjMesh()) {
		if (node->GetShader()) {
			SetShaderLight(*light);
			SetCurrentShader(node->GetShader());
			modelMatrix = node->GetWorldTransform() * Matrix4::Scale(node->GetModelScale());
			UpdateShaderMatrices();
			if (node->getObjMesh() != nullptr) {
				for (Mesh* m : node->getObjMesh()->getChilds()) {
					BindTextureToSamplerAndUniform(6, m->GetTexture(), "diffuseTex", currentShader, GL_TEXTURE_2D);
					BindTextureToSamplerAndUniform(5, terrain->getDeptMap(), "deptTex", currentShader, GL_TEXTURE_2D);
					glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "height"), terrain->getHeight());
					m->Draw();
				}
				node->getObjMesh()->Draw();
			} else {
				BindTextureToSamplerAndUniform(6, node->GetMesh()->GetTexture(), "diffuseTex", currentShader, GL_TEXTURE_2D);
				BindTextureToSamplerAndUniform(5, terrain->getDeptMap(), "deptTex", currentShader, GL_TEXTURE_2D);
				glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "height"), terrain->getHeight());
				node->GetMesh()->Draw();
			}
		}
	}
	glUseProgram(0);
	for (vector<SceneNode*>::const_iterator i = node->getChildIteratorStart(); i != node->getChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}
}

void Renderer::configureScene()
{
	OBJMesh* mesh = new OBJMesh();
	mesh->LoadOBJMesh(MESHDIR"Tree3.obj");
	vector<Mesh*> childs = mesh->getChilds();
	if (childs.size() > 0) {
		mesh->setTexture(SOIL_load_OGL_texture(TEXTUREDIR"grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		SetTextureRepeating(mesh->GetTexture(), true);
		childs[0]->setTexture(SOIL_load_OGL_texture(TEXTUREDIR"grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		SetTextureRepeating(childs[0]->GetTexture(), true);
		//childs[1]->setTexture(SOIL_load_OGL_texture(TEXTUREDIR"grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		//SetTextureRepeating(childs[1]->GetTexture(), true);
	} else {
		mesh->setTexture(SOIL_load_OGL_texture(TEXTUREDIR"grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		SetTextureRepeating(mesh->GetTexture(), true);
	}
	

	
	root = new SceneNode();

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);
	root->SetTransform(Matrix4::Translation(Vector3(heightX, 0, heightZ)));
	root->SetShader(testShader);
	float scale = 100.0f;
	for (int i = 0; i < 1000; i++) {

		SceneNode* tree = new SceneNode();
		tree->setObjMesh(mesh);
		tree->SetShader(testShader);
		tree->SetModelScale(Vector3(scale, scale, scale));
		tree->SetTransform(Matrix4::Translation(Vector3(RandomFloat(-heightX, heightX), 0, RandomFloat(-heightZ, heightZ))));

		root->AddChild(tree);
	}
}

float Renderer::RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}