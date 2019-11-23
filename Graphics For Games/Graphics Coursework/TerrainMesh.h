#pragma once
#include "../../nclgl/Mesh.h"
#include "Loader.h"

#define RAW_WIDTH 1025
#define RAW_HEIGHT 1025

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

#define HEIGHT_MAX 2000.0f

class TerrainMesh : public Mesh
{
public:
	TerrainMesh();
	~TerrainMesh();

	void Draw();
};
