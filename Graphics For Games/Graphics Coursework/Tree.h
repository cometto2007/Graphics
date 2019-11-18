#pragma once
#include "../../nclgl/GrowingMesh.h"
#include "../../nclgl/OBJMesh.h"

class Tree: public GrowingMesh
{
public:
	Tree(float speed, float maxHeight, float currentHeight);

	OBJMesh* getMesh();
private:
	OBJMesh* mesh;
};

