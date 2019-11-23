#pragma once
#include "../../nclgl/MD5Mesh.h"
#include "Utility.h"

#include "Loader.h"

class Bird :
	public MD5Node
{
public:
	Bird(const MD5FileData& ofType, float speed);

	virtual void Draw(const OGLRenderer& r);
	virtual void Update(float msec);

	float getSpeed() { return speed; }
	void setSpeed(float speed) { this->speed = speed; }
private:
	Loader loader = loader.getInstance();
	float speed;
};

