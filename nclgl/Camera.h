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

struct CameraData2 {
	Vector3 direction;
	Vector3 position;

	CameraData2(Vector3 direction, Vector3 position) {
		this->direction = direction;
		this->position = position;
	}

	CameraData2() : position{ 10,10,10 } {
		direction.ToZero();
	}

	CameraData2(const CameraData2& x) {
		direction = x.direction;
		position = x.position;
	}

	CameraData2& operator=(const CameraData2& x) {
		direction = x.direction;
		position = x.position;
		return *this;
	}
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

	bool isCam2Active = false;
	CameraData2 cam2;

	void setData2(const CameraData2& data) {
		cam2 = data;
		isCam2Active = true;
	}

	void addCameraConf(CameraData cd) { cameraConfs.push_back(cd); };
	void addCameraConf2(const Vector3& a, const  Vector3& b) { cameraConfs2.emplace_back(a, b); };
	void incrCameraIndex() { cameraConfsIndex++; };
	void setCameraIndex(int index) { 
		/*if (index <= cameraConfs.size() - 1) {
			cameraConfsIndex = index;
			setData(cameraConfs[index]);
		}*/
		cameraConfsIndex = index;
#if 1
		setData(cameraConfs[index]);
#else
		setData2(cameraConfs2[index]);
#endif
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
	vector<CameraData2> cameraConfs2;
	int cameraConfsIndex;
	int countFrame;
	float speed;
	bool autoMov;
};