#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float msec)	{
	//Update the mouse by how much

	Vector2 v1 = Window::GetMouse()->GetRelativePosition();
	
	pitch -= v1.y;
	yaw	  -= v1.x;

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch,90.0f);
	pitch = max(pitch,-90.0f);

	if(yaw <0) {
		yaw+= 360.0f;
	}
	if(yaw > 360.0f) {
		yaw -= 360.0f;
	}

	msec *= 5.0f;

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position += Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= msec;
	}
	 /*
	cout << "position: " << position << endl;
	cout << "yaw: " << yaw << endl;
	cout << "pitch: " << pitch << endl;*/
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return Matrix4::Rotation(-pitch, Vector3(1,0,0)) * 
			Matrix4::Rotation(-yaw, Vector3(0,1,0)) * 
			Matrix4::Translation(-position);
};

CameraEffects Camera::moveCameraAuto(float msec)
{
	CameraEffects effects = cameraConfs[cameraConfsIndex].effects;
	CameraData cd1 = cameraConfs[cameraConfsIndex].data;
	CameraData cd2 = cameraConfs[cameraConfsIndex + 1].data;
	CameraData pos;
	double distance = (cd1.position - cd2.position).Length();
	double time = distance / effects.speed;
	static double currentTime = 0;
	currentTime += msec;

	if (currentTime <= time) {
		double t = currentTime / time;
		pos.position.x = (t * (double)cd2.position.x) + ((1 - t) * (double)cd1.position.x);
		pos.position.y = (t * (double)cd2.position.y) + ((1 - t) * (double)cd1.position.y);
		pos.position.z = (t * (double)cd2.position.z) + ((1 - t) * (double)cd1.position.z);
		pos.pitch = (t * cd2.pitch) + ((1 - t) * cd1.pitch);
		pos.yaw = LerpDegrees(cd1.yaw, cd2.yaw, t);

		countFrame++;
		setData(pos);
	} else {
		if ((cameraConfsIndex + 1) <= (cameraConfs.size() - 2)) {
			cameraConfsIndex++;
		} else {
			autoMov = false;
		}
		countFrame = 0;
		currentTime = 0;
	}
	return effects;
}

float Camera::LerpDegrees(float start, float end, float amount)
{
	float difference = abs(end - start);
	if (difference > 180) {
		if (end > start) {
			start += 360;
		} else {
			end += 360;
		}
	}
	float value = (start + ((end - start) * amount));
	float rangeZero = 360;
	if (value >= 0 && value <= 360)
		return value;
	return fmod(value, rangeZero);
}
