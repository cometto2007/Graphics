#pragma once

#include "Terrain.h"
#include "../../nclgl/Matrix4.h"
#include "../../nclgl/Vector3.h"
#include "../../nclgl/Camera.h"


class MousePicker
{
public:
	MousePicker(Camera* cam, Matrix4 projection, Terrain* terrain);
	Vector3 getRay() { return ray; };
	Vector3 getCurrTerrainPoint() { return currTerrainPoint; };
	void update();

private:
	const int RECURSION_COUNT = 200;
	const float RAY_RANGE = 600.0f;

	Vector3 ray;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Camera* camera;
	Terrain* terrain;
	Vector3 currTerrainPoint;

	Vector3 calculateMouseRay();
	Vector2 getNormalizedDeviceCoords(Vector2 mousePos);
	Vector4 toEyeCoords(Vector4 clipCoords);
	Vector3 toWorldCoords(Vector4 eyeCoords);
	bool intersectionInRange(float start, float finish, Vector3 ray);
	Vector3 binarySearch(int count, float start, float finish, Vector3 ray);
	bool isUnderGround(Vector3 testPoint);
	Vector3 getPointOnRay(Vector3 ray, float distance);
};

