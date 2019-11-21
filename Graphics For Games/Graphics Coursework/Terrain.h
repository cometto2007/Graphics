#pragma once
#include "../../nclgl/Mesh.h"
#include "Loader.h"

#define RAW_WIDTH 513
#define RAW_HEIGHT 513

#define HEIGHTMAP_X 32.0f
#define HEIGHTMAP_Z 32.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 32.0f
#define HEIGHTMAP_TEX_Z 1.0f / 32.0f

#define HEIGHT_MAX 2000.0f

class Terrain : public Mesh
{
public:
	Terrain(float speed, float maxHeight, float currentHeight);
	~Terrain();

	void Draw();

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

	float getHeight();
	void updateHeight();

	virtual void Draw(const OGLRenderer& r);

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

	float speed;
	float currentHeight;
	float maxHeight;

	GLuint deptMapFBO;
};
