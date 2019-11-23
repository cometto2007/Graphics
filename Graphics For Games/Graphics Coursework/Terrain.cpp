#include "TerrainMesh.h"
#include "Terrain.h"

Terrain::Terrain(float speed, float maxHeight, float currentHeight)
{
	this->speed = speed;
	this->maxHeight = maxHeight;
	this->currentHeight = currentHeight;

	mesh = new TerrainMesh();

	shader = loader.light_shader();
	shadowShader = loader.test1_shader();
}

Terrain::~Terrain()
{
}

float Terrain::getHeight()
{
	return currentHeight;
}

void Terrain::updateHeight()
{
	float newHeight = currentHeight + speed;
	if (newHeight <= maxHeight) {
		currentHeight = newHeight;
	}
	else {
		currentHeight = maxHeight;
	}
}

void Terrain::Draw(const OGLRenderer& r)
{
	Loader loader = Loader::getInstance();

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(), "height"), getHeight());
	r.BindTextureToSamplerAndUniform(0, loader.getGroundTex(), "diffuseTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(1, loader.getBumpMap(), "bumpTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(2, loader.getDeptMap(), "deptTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(3, loader.getGrassMap(), "grassMap", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(4, loader.getSandMap(), "sandMap", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(5, loader.getSandWetMap(), "sandWetMap", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(6, loader.getCliffsMap(), "cliffsMap", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(7, loader.getGrassTex(), "grassTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(8, loader.getSandText(), "sandTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(9, loader.getSandWetText(), "sandWetTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(10, loader.getCliffsText(), "cliffsTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(12, loader.getGrassBumpMap(), "grassBumpMap", r.GetCurrentShader(), GL_TEXTURE_2D);
	
	mesh->Draw();
}

void Terrain::Update(float msec)
{
	updateHeight();
	SceneNode::Update(msec);
}
