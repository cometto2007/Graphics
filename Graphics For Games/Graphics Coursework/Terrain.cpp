#include "Terrain.h"

Terrain::Terrain(float speed, float maxHeight, float currentHeight) : GrowingMesh(speed, maxHeight, currentHeight)
{
	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	for (int x = 0; x < RAW_WIDTH; ++x) {
		for (int z = 0; z < RAW_HEIGHT; ++z) {
			int offset = (x * RAW_WIDTH) + z;
			vertices[offset] = Vector3(x * HEIGHTMAP_X, 0, z * HEIGHTMAP_Z);
			textureCoords[offset] = Vector2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
		}
	}

	numIndices = 0;

	for (int x = 0; x < RAW_WIDTH - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT - 1; ++z) {
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x + 1) * (RAW_WIDTH)) + z;
			int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
			int d = (x * (RAW_WIDTH)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	GenerateNormals();
	GenerateTangents();

	BufferData();

	// Set Texture
	setTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setGrassText(SOIL_load_OGL_texture(TEXTUREDIR"grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setCliffsText(SOIL_load_OGL_texture(TEXTUREDIR"cliffs.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setSandWetText(SOIL_load_OGL_texture(TEXTUREDIR"sandWet.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setSandText(SOIL_load_OGL_texture(TEXTUREDIR"sand.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	

	// Set Map
	setDeptMap(SOIL_load_OGL_texture(TEXTUREDIR"canyonIsland_heightmap_little.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setGrassMap(SOIL_load_OGL_texture(TEXTUREDIR"CI_Shared_Grass_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setCliffsMap(SOIL_load_OGL_texture(TEXTUREDIR"CI_Shared_Cliffs_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setSandWetMap(SOIL_load_OGL_texture(TEXTUREDIR"CI_Beach_Sands_Wet.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setSandMap(SOIL_load_OGL_texture(TEXTUREDIR"CI_Beach_Sands_02.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	

	if (!GetTexture() || !getGrassText() || !getCliffsText() || !getSandWetText() || !getSandText() || !GetBumpMap()) {
		return;
	}

	if (!getDeptMap() || !getGrassMap() || !getCliffsMap() || !getSandWetMap() || !getSandMap()) {
		return;
	}

}

Terrain::~Terrain()
{
	glDeleteTextures(1, &deptMap);
}

void Terrain::Draw()
{
	glBindVertexArray(arrayObject);
	if (bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(type, 0, numVertices);
	}
	glBindVertexArray(0);
}

