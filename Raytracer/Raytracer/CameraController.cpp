#include "CameraController.h"

void CameraController::moveForward(float distance) const
{
	camera.position = camera.position + camera.forward * distance;
}

void CameraController::moveRight(float distance) const
{
	camera.position = camera.position + camera.right * distance;
}

void CameraController::moveUp(float distance) const
{
	camera.position = camera.position + camera.up * distance;
}
