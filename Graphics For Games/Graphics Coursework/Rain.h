#pragma once
#include "../../nclgl/SceneNode.h"
#include "Utility.h"
#include "Loader.h"
#include "Terrain.h"


class Rain : public SceneNode
{
public:
	Rain();

	void Update(float msec) override;
private:
	Loader loader = loader.getInstance();
};

