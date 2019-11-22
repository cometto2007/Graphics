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

	cout << "position: " << position << endl;
	cout << "yaw: " << yaw << endl;
	cout << "pitch: " << pitch << endl;
}

Matrix4 lookAt(Vector3& eye, Vector3& target, Vector3& upDir)
{
	// compute the forward vector from target to eye
	Vector3 forward = eye - target;
	forward.Normalise();                 // make unit length

	// compute the left vector
	Vector3 left = Vector3::Cross(upDir,forward); // cross product
	left.Normalise();

	// recompute the orthonormal up vector
	Vector3 up = Vector3::Cross(forward,left);    // cross product

	// init 4x4 matrix
	float matrix[16];
	matrix[3] = 0.0f;
	matrix[6] = 0.0f;
	matrix[9] = 0.0f;

	// set rotation part, inverse rotation matrix: M^-1 = M^T for Euclidean transform
	matrix[0] = left.x;
	matrix[4] = left.y;
	matrix[8] = left.z;
	matrix[1] = up.x;
	matrix[5] = up.y;
	matrix[9] = up.z;
	matrix[2] = forward.x;
	matrix[6] = forward.y;
	matrix[10] = forward.z;

	// set translation part
	matrix[12] = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
	matrix[13] = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
	matrix[14] = -forward.x * eye.x - forward.y * eye.y - forward.z * eye.z;

	return Matrix4(matrix);
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return !isCam2Active ?  Matrix4::Rotation(-pitch, Vector3(1,0,0)) * 
			Matrix4::Rotation(-yaw, Vector3(0,1,0)) * 
		Matrix4::Translation(-position) : lookAt(cam2.position, cam2.direction, Vector3(0,1,0));
};

void Camera::moveCameraAuto(float msec)
{
#if 1
	if (autoMov) {
		CameraData cd1 = cameraConfs[cameraConfsIndex];
		CameraData cd2 = cameraConfs[cameraConfsIndex + 1];
		CameraData pos;
		double distance = (cd1.position - cd2.position).Length();
		double time = distance / speed;
		static double currentTime = 0;
		currentTime += msec;
		//int cameraStopPoint = floor(time / msec);

		if (currentTime <= time) {
			double t = currentTime / time;
			pos.position.x = (t * (double)cd2.position.x) + ((1 - t) * (double)cd1.position.x);
			pos.position.y = (t * (double)cd2.position.y) + ((1 - t) * (double)cd1.position.y);
			pos.position.z = (t * (double)cd2.position.z) + ((1 - t) * (double)cd1.position.z);
			//pos.pitch = (t * cd2.pitch) + ((1 - t) * cd1.pitch);
			//pos.yaw = (t * cd2.yaw) + ((1 - t) * cd1.yaw);
			pos.pitch = cd1.pitch;
			pos.yaw = cd1.yaw;

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
	}
#else
	if (autoMov) {
		CameraData2 cd1 = cameraConfs2[cameraConfsIndex];
		CameraData2 cd2 = cameraConfs2[cameraConfsIndex + 1];
		CameraData2 pos;
		float distance1 = (cd1.position - cd2.position).Length();
		float distance2 = (cd1.direction - cd2.direction).Length();
		//float time = distance / speed;
		int cameraStopPoint1 = floor((distance1 / speed) / msec);
		int cameraStopPoint2 = floor((distance2 / speed) / msec);

		if (countFrame < cameraStopPoint1) {
			float t = (1.0f / (float)cameraStopPoint1) * (float)countFrame;
			float t2 = (1.0f / (float)cameraStopPoint2) * (float)countFrame;
			pos.position.x = (t * cd2.position.x) + ((1 - t) * cd1.position.x);
			pos.position.y = (t * cd2.position.y) + ((1 - t) * cd1.position.y);
			pos.position.z = (t * cd2.position.z) + ((1 - t) * cd1.position.z);

			pos.direction.x = (t2 * cd2.direction.x) + ((1 - t2) * cd1.direction.x);
			pos.direction.y = (t2 * cd2.direction.y) + ((1 - t2) * cd1.direction.y);
			pos.direction.z = (t2 * cd2.direction.z) + ((1 - t2) * cd1.direction.z);
			//pos.pitch = (t * cd2.pitch) + ((1 - t) * cd1.pitch);
			//pos.yaw = (t * cd2.yaw) + ((1 - t) * cd1.yaw);
			//pos.pitch = cd1.pitch;
			//pos.yaw = cd1.yaw;
			countFrame++;
			setData2(pos);
		}
		else {
			if ((cameraConfsIndex + 1) <= (cameraConfs.size() - 2)) {
				cameraConfsIndex++;
			}
			else {
				autoMov = false;
			}
			countFrame = 0;
		}
}
#endif
}
