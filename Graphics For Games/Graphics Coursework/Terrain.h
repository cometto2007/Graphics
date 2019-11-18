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

	GLuint getDeptMap() { return deptMap; };
	void setDeptMap(GLuint deptMap) { this->deptMap = deptMap;  };

	GLuint getGrassMap() { return grassMap; };
	void setGrassMap(GLuint grassMap) { this->grassMap = grassMap; };

	GLuint getCliffsMap() { return cliffsMap; };
	void setCliffsMap(GLuint cliffsMap) { this->cliffsMap = cliffsMap; };

	GLuint getSandWetMap() { return sandWetMap; };
	void setSandWetMap(GLuint sandWetMap) { this->sandWetMap = sandWetMap; };

	GLuint getSandMap() { return sandMap; };
	void setSandMap(GLuint sandMap) { this->sandMap = sandMap; };

	GLuint getGrassText() { return grassText; };
	void setGrassText(GLuint grassText) { this->grassText = grassText; };

	GLuint getCliffsText() { return cliffsText; };
	void setCliffsText(GLuint cliffsText) { this->cliffsText = cliffsText; };

	GLuint getSandWetText() { return sandWetText; };
	void setSandWetText(GLuint sandWetText) { this->sandWetText = sandWetText; };

	GLuint getSandText() { return sandText; };
	void setSandText(GLuint sandText) { this->sandText = sandText; };

private:
	GLuint deptMap;
	GLuint grassMap;
	GLuint cliffsMap;
	GLuint sandWetMap;
	GLuint sandMap;

	GLuint grassText;
	GLuint cliffsText;
	GLuint sandWetText;
	GLuint sandText;

};
