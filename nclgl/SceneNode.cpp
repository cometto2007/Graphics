#include "SceneNode.h"

SceneNode::SceneNode(Mesh* m, Vector4 colour, OBJMesh* mesh)
{
	this->mesh = m;
	this->objMesh = mesh;
	this->colour = colour;
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	parent = NULL;
	modelScale = Vector3(1,1,1);
}

SceneNode::~SceneNode(void)
{
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s)
{
	children.push_back(s);
	s->parent = this;
}

void SceneNode::Update(float msec)
{
	if (parent) {
		worldTransform = parent->worldTransform * transform;
	} else {
		worldTransform = transform;
	}
	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void SceneNode::Draw(const OGLRenderer& r)
{
	if (mesh) { mesh->Draw(); }
}
