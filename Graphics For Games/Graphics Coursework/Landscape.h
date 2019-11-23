#pragma once
#include "../../nclgl/SceneNode.h"
#include "Loader.h"
class Landscape :
	public SceneNode
{
public:
	Landscape();
private:
	Loader loader = Loader::getInstance();
};

