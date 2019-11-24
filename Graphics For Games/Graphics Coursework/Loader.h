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
	GLuint getSkyBox() { return skyBox; };
	Shader* light_shader() const { return lightShader; }
	Shader* reflect_shader() const { return reflectShader; }
	Shader* skybox_shader() const { return skyboxShader; }
	Shader* test_shader() const { return testShader; }
	Shader* shadow_shader() const { return shadowShader; }
	Shader* scene_shader() const { return sceneShader; }
	Shader* test1_shader() const { return test1Shader; }
	Shader* scene_node_shadow_shader() const { return sceneNodeShadowShader; }
	Shader* calc_shadow_grass_field() const { return calcShadowGrassField; }
	Shader* process_shader() const { return processShader; }
	Shader* post_process_shader() const { return postProcessShader; }
	Shader* bird_shadow_shader() const { return birdShadowShader; }
	Shader* animation_shader() const { return animationShader; }

private:
	Loader();

	OBJMesh* tree;

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* testShader;
	Shader* shadowShader;
	Shader* sceneShader;
	Shader* test1Shader;
	Shader* sceneNodeShadowShader;
	Shader* calcShadowGrassField;
	Shader* processShader;
	Shader* postProcessShader;
	Shader* birdShadowShader;
	Shader* animationShader;

	GLuint grassTex;
	GLuint barkTex;
	GLuint deptMap;
	GLuint grassMap;
	GLuint cliffsMap;
	GLuint sandWetMap;
	GLuint sandMap;
	GLuint bumpMap;
	GLuint skyBox;

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

