#include "Loader.h"

Loader::Loader()
{
	tree = new OBJMesh();
	tree->LoadOBJMesh(MESHDIR"Tree.obj");

	birdData = new MD5FileData(MESHDIR"test1.md5mesh");
	birdData->AddAnim(MESHDIR"test1.md5anim");

	barkTex = SOIL_load_OGL_texture(TEXTUREDIR"bark.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	groundTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassTex = SOIL_load_OGL_texture(TEXTUREDIR"grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cliffsTex = SOIL_load_OGL_texture(TEXTUREDIR"cliffs.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandWetTex = SOIL_load_OGL_texture(TEXTUREDIR"sandWet.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandTex = SOIL_load_OGL_texture(TEXTUREDIR"sand.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	bumpMap = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	// Set Map
	deptMap = SOIL_load_OGL_texture(TEXTUREDIR"heightMap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Shared_Grass_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cliffsMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Shared_Cliffs_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandWetMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Beach_Sands_Wet.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Beach_Sands_02.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	grassBumpMap = SOIL_load_OGL_texture(TEXTUREDIR"grassBumpMap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cliffsBumpMap = SOIL_load_OGL_texture(TEXTUREDIR"cliffsBumpMap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	SetTextureRepeating(grassTex, true);
	SetTextureRepeating(grassBumpMap, true);
	SetTextureRepeating(barkTex, true);
	SetTextureRepeating(groundTex, true);
	SetTextureRepeating(cliffsTex, true);
	SetTextureRepeating(sandWetTex, true);
	SetTextureRepeating(sandTex, true);
	SetTextureRepeating(bumpMap, true);
	SetTextureRepeating(waterTex, true);

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

	skyBox = SOIL_load_OGL_cubemap(TEXTUREDIR"niceday2ft.tga", TEXTUREDIR"niceday2bk.tga",
		TEXTUREDIR"niceday2up.tga", TEXTUREDIR"niceday2dn.tga",
		TEXTUREDIR"niceday2rt.tga", TEXTUREDIR"niceday2lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);
}

void Loader::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}
