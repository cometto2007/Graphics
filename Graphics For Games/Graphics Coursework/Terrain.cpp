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

	setTexture(SOIL_load_OGL_texture(TEXTUREDIR"TexturesCom_Grass0130_1_seamless_S.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setGrassText(SOIL_load_OGL_texture(TEXTUREDIR"TexturesCom_Grass0130_1_seamless_S.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setDeptText(SOIL_load_OGL_texture(TEXTUREDIR"canyonIsland_heightmap_little.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	setBlendMap(SOIL_load_OGL_texture(TEXTUREDIR"BlendMap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (GetTexture() || GetBumpMap() || getDeptText() || getGrassText() || getBlendMap()) {
		return;
	}
}

Terrain::~Terrain()
{
	glDeleteTextures(1, &deptText);
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

GLuint Terrain::getDeptText()
{
	return deptText;
}

void Terrain::setDeptText(GLuint deptText)
{
	this->deptText = deptText;
}

void Terrain::setGrassText(GLuint grassText)
{
	this->grassText = grassText;
}

GLuint Terrain::getBlendMap()
{
	return blendMap;
}

void Terrain::setBlendMap(GLuint blendMap)
{
	this->blendMap = blendMap;
}

GLuint Terrain::getGrassText()
{
	return grassText;
}
