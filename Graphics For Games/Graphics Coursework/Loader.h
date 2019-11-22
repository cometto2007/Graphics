#pragma once
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"

class Loader
{
public:
	static Loader& getInstance()
	{
		static Loader instance; 
		return instance;
	}
	MD5FileData* getBirdData(){ return birdData; }
	OBJMesh* getTree() { return tree; };
	GLuint getGrassTex() { return grassTex; };
	GLuint getBarkTex() { return barkTex; };
	GLuint getDeptMap() { return deptMap; };
	GLuint getGrassMap() { return grassMap; };
	GLuint getCliffsMap() { return cliffsMap; };
	GLuint getSandWetMap() { return sandWetMap; };
	GLuint getSandMap() { return sandMap; };
	GLuint getCliffsText() { return cliffsTex; };
	GLuint getSandWetText() { return sandWetTex; };
	GLuint getSandText() { return sandTex; };
	GLuint getBumpMap() { return bumpMap; };
	GLuint getGroundTex() { return groundTex; };
	GLuint getWaterTex() { return waterTex; };
	GLuint getGrassBumpMap() { return grassBumpMap; };
	GLuint getCliffsBumpMap() { return cliffsBumpMap; };

private:
	Loader();

	OBJMesh* tree;

	GLuint grassTex;
	GLuint barkTex;
	GLuint deptMap;
	GLuint grassMap;
	GLuint cliffsMap;
	GLuint sandWetMap;
	GLuint sandMap;
	GLuint bumpMap;

	GLuint cliffsTex;
	GLuint sandWetTex;
	GLuint sandTex;
	GLuint groundTex;
	GLuint waterTex;

	GLuint grassBumpMap;
	GLuint cliffsBumpMap;

	MD5FileData* birdData;

	void SetTextureRepeating(GLuint target, bool repeating);
};

