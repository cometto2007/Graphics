#pragma once
#include "../../nclgl/Mesh.h"

#define RAW_WIDTH 513
#define RAW_HEIGHT 513

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

class Terrain : public Mesh
{
public:
	Terrain();
	~Terrain();

	void Draw() override;
	GLuint getDeptText();
	void setDeptText(GLuint deptText);

private:
	GLuint deptText;
	GLuint terrainText;
};

