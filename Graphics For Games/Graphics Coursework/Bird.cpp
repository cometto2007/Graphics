#include "Bird.h"

Bird::Bird(const MD5FileData& ofType, float speed) : MD5Node(ofType)
{
	this->speed = speed;
	PlayAnim(MESHDIR"test1.md5anim");
}

void Bird::Draw(const OGLRenderer& r)
{
	MD5Node::Draw(r);
}

void Bird::Update(float msec)
{
	transform = transform * Matrix4::Rotation(0.2, Vector3(0.0f, 0.0f, 1.0f)) *
		Matrix4::Translation(Vector3(RandomFloat(-speed * msec, speed * msec) / 10, speed * msec, 0.0f));
	MD5Node::Update(RandomFloat(0, msec * 5));
}

float Bird::RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
