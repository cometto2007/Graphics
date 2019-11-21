#include "Loader.h"

Loader::Loader()
{
	tree = new OBJMesh();
	tree->LoadOBJMesh(MESHDIR"Tree.obj");

	barkTex = SOIL_load_OGL_texture(TEXTUREDIR"bark.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	groundTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassTex = SOIL_load_OGL_texture(TEXTUREDIR"grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cliffsTex = SOIL_load_OGL_texture(TEXTUREDIR"cliffs.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandWetTex = SOIL_load_OGL_texture(TEXTUREDIR"sandWet.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandTex = SOIL_load_OGL_texture(TEXTUREDIR"sand.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	bumpMap = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	// Set Map
	deptMap = SOIL_load_OGL_texture(TEXTUREDIR"heightMap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Shared_Grass_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cliffsMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Shared_Cliffs_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandWetMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Beach_Sands_Wet.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandMap = SOIL_load_OGL_texture(TEXTUREDIR"CI_Beach_Sands_02.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	SetTextureRepeating(grassTex, true);
	SetTextureRepeating(barkTex, true);
	SetTextureRepeating(groundTex, true);
	SetTextureRepeating(cliffsTex, true);
	SetTextureRepeating(sandWetTex, true);
	SetTextureRepeating(sandTex, true);
	SetTextureRepeating(bumpMap, true);
}

void Loader::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}
