#include "Water.h"

Water::Water(GLuint cubeMap)
{
	waterRotate = 0.0f;
	mesh = Mesh::GenerateQuad();
	mesh->setTexture(loader.getWaterTex());

	this->cubeMap = cubeMap;
	SetShader(loader.reflect_shader());
	SetShadowShader(loader.shadow_shader());
}

Water::~Water()
{
}

void Water::Update(float msec)
{
	SceneNode::Update(msec);
	waterRotate += msec / 5000.0f;
	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(1.0f, 1.0f, 1.0f));
}

void Water::Draw(const OGLRenderer& r)
{
	r.BindTextureToSamplerAndUniform(2, cubeMap, "cubeTex", r.GetCurrentShader(), GL_TEXTURE_CUBE_MAP);
	r.BindTextureToSamplerAndUniform(4, loader.getWaterTex(), "diffuseTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	mesh->Draw();
}
