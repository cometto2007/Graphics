#include "Bird.h"

Bird::Bird(const MD5FileData& ofType, float speed) : MD5Node(ofType)
{
	shader = loader.animation_shader();
	shadowShader = loader.bird_shadow_shader();
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
		Matrix4::Translation(Vector3(Utility::RandomFloat(-speed * msec, speed * msec) / 10, speed * msec, 0.0f));
	MD5Node::Update(Utility::RandomFloat(0, msec * 5));
}
