/******************************************************************************
Class:Camera
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"

#define SPEED 2.0f

struct CameraData {
	float pitch;
	float yaw;
	Vector3 position;
};

class Camera	{
public:
	Camera(void){
		yaw		= 0.0f;
		pitch	= 0.0f;
		cameraConfsIndex = -1;
		countFrame = 1;
		speed = SPEED;
		autoMov = false;
	};

	Camera(CameraData data) {
		this->pitch = data.pitch;
		this->yaw = data.yaw;
		this->position = data.position;

		cameraConfsIndex = -1;
		countFrame = 1;
		speed = SPEED;
		autoMov = false;
	}

	Camera(float pitch, float yaw, Vector3 position){
		this->pitch		= pitch;
		this->yaw		= yaw;
		this->position	= position;

		cameraConfsIndex = -1;
		countFrame = 1;
		speed = SPEED;
		autoMov = false;
	}

	~Camera(void){};

	void UpdateCamera(float msec = 10.0f);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 BuildViewMatrix();

	//Gets position in world space
	Vector3 GetPosition() const { return position;}
	//Sets position in world space
	void	SetPosition(Vector3 val) { position = val;}

	//Gets yaw, in degrees
	float	GetYaw()   const { return yaw;}
	//Sets yaw, in degrees
	void	SetYaw(float y) {yaw = y;}

	//Gets pitch, in degrees
	float	GetPitch() const { return pitch;}
	//Sets pitch, in degrees
	void	SetPitch(float p) {pitch = p;}

	void setData(CameraData data) {
		this->pitch = data.pitch;
		this->yaw = data.yaw;
		this->position = data.position;
	}

	void addCameraConf(CameraData cd) { cameraConfs.push_back(cd); };
	void incrCameraIndex() { cameraConfsIndex++; };
	void setCameraIndex(int index) { 
		if (index <= cameraConfs.size() - 1) {
			cameraConfsIndex = index;
			setData(cameraConfs[index]);
		}
	};
	int getCameraIndex() { return cameraConfsIndex; };
	void setCameraSpeed(int speed) { this->speed = speed; };

	void moveCameraAuto(float msec);
	void setAutoCam(bool autoMov) { this->autoMov = autoMov; }

	void printData() {
		cout << "yaw: " << yaw << endl;
		cout << "pitch: " << pitch << endl;
		cout << "position: " << position << endl;
	}

protected:
	float	yaw;
	float	pitch;
	Vector3 position;

	vector<CameraData> cameraConfs;
	int cameraConfsIndex;
	int countFrame;
	float speed;
	bool autoMov;
};