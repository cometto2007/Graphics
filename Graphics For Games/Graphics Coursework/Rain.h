#pragma once
#include "../../nclgl/SceneNode.h"
#include "Utility.h"
#include "Loader.h"
#include "Terrain.h"


class Rain : public SceneNode
{
public:
	Rain();

	void Draw(const OGLRenderer& r) override;
	void Update(float msec) override;
	void updateDrawing(int i) override;
	
private:
	const int dripNumb = 1000;
	const float speed = 500.0f;
	Loader loader = loader.getInstance();

	vector<Matrix4> pos;
};

