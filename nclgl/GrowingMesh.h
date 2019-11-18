#pragma once
#include "Mesh.h"

class GrowingMesh: public Mesh
{
public:
	GrowingMesh(float speed, float maxHeight, float currentHeight);

	float getHeight();
	void updateHeight();
protected:
	float speed;
	float currentHeight;
	float maxHeight;
};

