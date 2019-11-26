#include "Landscape.h"
#include "Skybox.h"
#include "Tree.h"
#include "Utility.h"
#include "Bird.h"
#include "Water.h"

Landscape::Landscape()
{
	terrain = new Terrain(3.0f, 3000.0f, 0.0f);

	Skybox* skyBox = new Skybox();
	skyBox->SetBoundingRadius(100000.0f);
	AddChild(skyBox);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X);
	SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	SetShader(loader.test_shader());

	float scale = 80.0f;
	for (int i = 0; i < 100; i++) {
		Tree* tree = new Tree();
		tree->SetModelScale(Vector3(scale, scale, scale));
		tree->SetTransform(Matrix4::Translation(Vector3(Utility::RandomFloat(0, heightX), 0, Utility::RandomFloat(0, heightX))));
		tree->SetBoundingRadius(100000.0f);
		
		skyBox->AddChild(tree);
	}

	scale = 0.1f;
	birds = new SceneNode();
	birds->SetTransform(Matrix4::Translation(Vector3(heightX, 2500.0f, heightX)));
	birds->SetShader(loader.test_shader());
	for (int i = 0; i < 15; i++) {
		Bird* bird = new Bird(*loader.getBirdData(), Utility::RandomFloat(0.4f, 0.6f));
		bird->SetModelScale(Vector3(scale, scale, scale));
		bird->SetTransform(Matrix4::Rotation(-90, Vector3(1.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90, Vector3(0.0f, 0.0f, 1.0f)) *
			Matrix4::Translation(Vector3(heightX / 2, 0, Utility::RandomFloat(0, 50.0f))));
		bird->SetBoundingRadius(100000.0f);
		birds->AddChild(bird);
	}

	skyBox->AddChild(birds);

	Water* water = new Water(skyBox->cube_map());
	water->SetModelScale(Vector3(heightX / 2.0f * 10, heightX / 2.0f * 10, 1));
	water->SetTransform(Matrix4::Translation(Vector3(heightX / 2.0f, 1550 * HEIGHTMAP_Y / 2.0f, heightX / 2.0f))* Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f)));
	water->SetBoundingRadius(100000.0f);
	water->setIsTransparent(true);
	
	skyBox->AddChild(water);
	terrain->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	terrain->SetBoundingRadius(10000000000.0f);

	skyBox->AddChild(terrain);
}
