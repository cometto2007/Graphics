#pragma once
#include "../../nclgl/MD5Mesh.h"
#include "../../nclgl/MD5Mesh.h"
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
	float speed;
	float RandomFloat(float a, float b);
};

