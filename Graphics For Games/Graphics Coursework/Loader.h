#pragma once
#include "../../nclgl/OBJMesh.h"

class Loader
{
public:
	static Loader& getInstance()
	{
		static Loader instance; 
		return instance;
	}
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
	
	void SetTextureRepeating(GLuint target, bool repeating);
};

