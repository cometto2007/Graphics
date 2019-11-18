#include "Tree.h"

Tree::Tree(float speed, float maxHeight, float currentHeight): GrowingMesh(speed, maxHeight, currentHeight)
{
	mesh = new OBJMesh();
	mesh->LoadOBJMesh(MESHDIR"Tree.obj");
	mesh->setTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
}

OBJMesh* Tree::getMesh()
{
	return mesh;
}
