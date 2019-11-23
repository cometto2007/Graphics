#pragma once
#include "../../nclgl/Mesh.h"
#include "../../nclgl/SceneNode.h"
#include "Loader.h"

#define RAW_WIDTH 1025
#define RAW_HEIGHT 1025

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

#define HEIGHT_MAX 2000.0f

class Terrain : public SceneNode
{
public:
	Terrain(float speed, float maxHeight, float currentHeight);
	~Terrain();

	float getHeight();
	void updateHeight();

	virtual void Draw(const OGLRenderer& r);
	void Update(float msec) override;

private:
	float speed;
	float currentHeight;
	float maxHeight;

	Loader loader = Loader::getInstance();
	
};
