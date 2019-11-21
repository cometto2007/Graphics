#pragma once
#include "../../nclgl/SceneNode.h"
#include "Loader.h"
#include "Renderer.h"
class Tree :
	public SceneNode
{
public:
	Tree();

	virtual void Draw(const OGLRenderer& r);
};

