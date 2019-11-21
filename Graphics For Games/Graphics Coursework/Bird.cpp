#include "Bird.h"

Bird::Bird(const MD5FileData& ofType, float speed) : MD5Node(ofType)
{
	this->speed = speed;
	PlayAnim(MESHDIR"test1.md5anim");
}

void Bird::Draw(const OGLRenderer& r)
{
	MD5Node::Draw(r);
	SetTransform((GetWorldTransform() * Matrix4::Translation(Vector3(0.0f, 0.0f, -speed))));
}
