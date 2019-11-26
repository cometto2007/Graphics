#include "Rain.h"
#include "Renderer.h"

Rain::Rain()
{
	mesh = Mesh::GeneratePoint();
	mesh->setTexture(loader.getWaterTex());
	setDrawingMesh(dripNumb);
	shader = loader.rain_shader();
	shadowShader = nullptr;

	float heightX = (RAW_WIDTH * HEIGHTMAP_X);
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z);

	pos = vector<Matrix4>(dripNumb);
	for (size_t i = 0; i < dripNumb; i++) {
		pos[i] = Matrix4::Translation(Vector3(Utility::RandomFloat(0, heightX), Utility::RandomFloat(0, 10000.0f), Utility::RandomFloat(0, heightZ))) *
			Matrix4::Scale(Vector3(2.5, 15, 10));
	}
}

void Rain::Draw(const OGLRenderer& r)
{
	mesh->Draw();
}

void Rain::Update(float msec)
{
	for (size_t i = 0; i < dripNumb; i++) {
		Matrix4 oldPos = pos[i];
		Matrix4 newPos = Matrix4::Translation(Vector3(0, -speed, 0)) * pos[i];
		pos[i] = newPos;
	}
}

void Rain::updateDrawing(int i)
{
	worldTransform = pos[i];
}
