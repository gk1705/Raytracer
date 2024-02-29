#include "Camera.h"

#include <SDL_stdinc.h>

Camera::Camera(): position(Vector3(0, 0, 0)), orientation(Quaternion()), forward(Vector3(0, 0, 1)), up(Vector3(0, 1, 0)), right(Vector3(1, 0, 0))
{
	updateDirectionVectors();
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
	const float yawRadians = deltaYaw * M_PI / 180.0f;
	const float pitchRadians = deltaPitch * M_PI / 180.0f;

	const Quaternion yawRotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), yawRadians); // yaw around the global up axis
	const Quaternion pitchRotation = Quaternion::fromAxisAngle(Vector3(1, 0, 0), pitchRadians); // pitch around the global right axis

	orientation = yawRotation * orientation * pitchRotation;
	orientation = orientation.normalized();

	updateDirectionVectors();
}

void Camera::updateDirectionVectors()
{
	forward = orientation * Vector3(0, 0, 1);
	right = orientation * Vector3(1, 0, 0);
	up = orientation * Vector3(0, 1, 0);

	forward.normalize();
	right.normalize();
	up.normalize();
}