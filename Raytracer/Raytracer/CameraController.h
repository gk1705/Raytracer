#pragma once
#include "Camera.h"

class CameraController
{
public:
	CameraController(Camera& camera, float speed)
		: camera(camera), speed(speed)
	{
	}

	void moveForward(float distance) const;
	void moveRight(float distance) const;
	void moveUp(float distance) const;

private:
	Camera& camera;
	float speed;
};
