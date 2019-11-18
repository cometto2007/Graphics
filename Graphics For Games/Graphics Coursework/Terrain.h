#pragma once
#include "../../nclgl/GrowingMesh.h"

#define RAW_WIDTH 513
#define RAW_HEIGHT 513

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

#define HEIGHT_MAX 2000.0f

class Terrain : public GrowingMesh
{
public:
	Terrain(float speed, float maxHeight, float currentHeight);
	~Terrain();

	void Draw() override;

	GLuint getDeptText();
	void setDeptText(GLuint deptText);

	GLuint getGrassText();
	void setGrassText(GLuint grassText);

	GLuint getBlendMap();
	void setBlendMap(GLuint blendMap);

private:
	GLuint deptText;
	GLuint terrainText;
	GLuint grassText;
	GLuint blendMap;
};
