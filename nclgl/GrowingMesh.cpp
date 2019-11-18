#include "GrowingMesh.h"

GrowingMesh::GrowingMesh(float speed, float maxHeight, float currentHeight): Mesh()
{
	this->speed = speed;
	this->maxHeight = maxHeight;
	this->currentHeight = currentHeight;
}

float GrowingMesh::getHeight()
{
	return currentHeight;
}

void GrowingMesh::updateHeight()
{
	float newHeight = currentHeight + speed;
	if (newHeight <= maxHeight) {
		currentHeight = newHeight;
	}
	else {
		currentHeight = maxHeight;
	}
}
