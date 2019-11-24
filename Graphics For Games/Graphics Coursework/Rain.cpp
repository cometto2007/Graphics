#include "Rain.h"

Rain::Rain()
{
	mesh = Mesh::GenerateQuad();
	mesh->setTexture(loader.getWaterTex());
	shader = loader.post_process_shader();
	shadowShader = nullptr;
}

void Rain::Update(float msec)
{
	float heightX = (RAW_WIDTH * HEIGHTMAP_X);
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z);

	transform = Matrix4::Translation(Vector3(Utility::RandomFloat(0, heightX), Utility::RandomFloat(0, 1000.0f), Utility::RandomFloat(0, heightZ))) *
		Matrix4::Scale(Vector3(50, 300, 200));
}
