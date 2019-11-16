#include "MousePicker.h"

MousePicker::MousePicker(Camera* cam, Matrix4 projection, Terrain* terrain)
{
	this->camera = cam;
	this->projectionMatrix = projectionMatrix;
	this->viewMatrix = camera->BuildViewMatrix();
	this->terrain = terrain;
}

void MousePicker::update()
{
	this->viewMatrix = camera->BuildViewMatrix();
	ray = calculateMouseRay();
	if (intersectionInRange(0, RAY_RANGE, ray)) {
		currTerrainPoint = binarySearch(0, 0, RAY_RANGE, ray);
	}
	else {
		currTerrainPoint = Vector3();
	}
}

Vector3 MousePicker::calculateMouseRay()
{
	Mouse* mouse = Window::GetMouse();
	Vector2 mousePos = mouse->GetAbsolutePosition();
	Vector2 normCoords = getNormalizedDeviceCoords(mousePos);
	Vector4 clipCoords = Vector4(normCoords.x, normCoords.y, -1.0f, 1.0f);
	Vector4 eyeCoords = toEyeCoords(clipCoords);
	Vector3 worldRay = toWorldCoords(eyeCoords);
	return worldRay;
}

Vector2 MousePicker::getNormalizedDeviceCoords(Vector2 mousePos)
{
	{
		Window* window = Window::GetWindow();
		// DOUBT!!!!
		Vector2 screen = window->GetScreenSize();
		float x = (2.0f * mousePos.x) / screen.x - 1.0f;
		float y = (2.0f * mousePos.y) / screen.y - 1.0f;
		// DOUBT!!! -y = y
		return Vector2(x, -y);
	}
}

Vector4 MousePicker::toEyeCoords(Vector4 clipCoords)
{
	Matrix4 invetedProj = projectionMatrix.Invert();
	Vector4 eyeCoords = invetedProj * clipCoords;
	return Vector4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

Vector3 MousePicker::toWorldCoords(Vector4 eyeCoords)
{
	Matrix4 invertedView = viewMatrix.Invert();
	Vector4 rayWorld = invertedView * eyeCoords;
	Vector3 mouseRay = Vector3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay.Normalise();
	return mouseRay;
}

bool MousePicker::intersectionInRange(float start, float finish, Vector3 ray)
{
	return false;
}

Vector3 MousePicker::binarySearch(int count, float start, float finish, Vector3 ray)
{
	float half = start + ((finish - start) / 2.0f);
	if (count >= RECURSION_COUNT) {
		Vector3 endPoint = getPointOnRay(ray, half);
	}
	if (intersectionInRange(start, half, ray)) {
		return binarySearch(count + 1, start, half, ray);
	}
	else {
		return binarySearch(count + 1, half, finish, ray);
	}
}

bool MousePicker::isUnderGround(Vector3 testPoint)
{
	float height = 0;
	//height = terrain.getHeightOfTerrain(testPoint.getX(), testPoint.getZ());
	if (testPoint.y < height) {
		return true;
	}
	else {
		return false;
	}
}

Vector3 MousePicker::getPointOnRay(Vector3 ray, float distance)
{
	Vector3 camPos = camera->GetPosition();
	Vector3 start = Vector3(camPos.x, camPos.y, camPos.z);
	Vector3 scaledRay = Vector3(ray.x * distance, ray.y * distance, ray.z * distance);
	return start + scaledRay;
}
