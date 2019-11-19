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
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1,0,0)) * 
			Matrix4::Rotation(-yaw, Vector3(0,1,0)) * 
			Matrix4::Translation(-position);
};

void Camera::moveCameraAuto(float msec)
{
	// TODO: add modularity for all the points
	if (autoMov) {
		CameraData cd1 = cameraConfs[cameraConfsIndex];
		CameraData cd2 = cameraConfs[cameraConfsIndex + 1];
		CameraData pos;
		float distance = (cd1.position - cd2.position).Length();
		float time = distance / speed;
		int cameraStopPoint = floor(time / msec);

		if (countFrame < cameraStopPoint) {
			float t = (1.0f / (float)cameraStopPoint) * (float)countFrame;
			pos.position.x = (t * cd2.position.x) + ((1 - t) * cd1.position.x);
			pos.position.y = (t * cd2.position.y) + ((1 - t) * cd1.position.y);
			pos.position.z = (t * cd2.position.z) + ((1 - t) * cd1.position.z);
			pos.pitch = (t * cd2.pitch) + ((1 - t) * cd1.pitch);
			pos.yaw = (t * cd2.yaw) + ((1 - t) * cd1.yaw);

			countFrame++;
			setData(pos);
		} else {
			if ((cameraConfsIndex + 1) <= (cameraConfs.size() - 2)) {
				cameraConfsIndex++;
			} else {
				autoMov = false;
			}
			countFrame = 0;
		}
	}
}
