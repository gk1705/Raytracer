#pragma once
#include "Vector3.h"

class Quaternion
{
public:
	float w, x, y, z;

	Quaternion() : w(1), x(0), y(0), z(0) {}
	Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

	static Quaternion fromAxisAngle(const Vector3& axis, float angle);
	Quaternion normalized() const;
	Quaternion operator*(const Quaternion& other) const;
	Vector3 operator*(const Vector3& vec) const;
};