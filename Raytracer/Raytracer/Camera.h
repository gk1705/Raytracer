#pragma once

#include "Quaternion.h"
#include "Vector3.h"

class Camera
{
public:
	Vector3 position;
	Vector3 forward;
	Vector3 up;
	Vector3 right;

	Camera();
	void rotate(float deltaYaw, float deltaPitch);
	void updateDirectionVectors();

private:
	Quaternion orientation;
};
