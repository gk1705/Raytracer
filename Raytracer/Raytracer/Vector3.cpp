#include "Vector3.h"

#include <cmath>

Vector3 Vector3::operator+(const Vector3& other) const
{
	return { x + other.x, y + other.y, z + other.z };
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return { x - other.x, y - other.y, z - other.z };
}

Vector3 Vector3::operator*(float scalar) const
{
	return { x * scalar, y * scalar, z * scalar };
}

float Vector3::operator*(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const
{
	return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
}

void Vector3::normalize()
{
	const float length = sqrt(x * x + y * y + z * z);
	x /= length;
	y /= length;
	z /= length;
}

Vector3 Vector3::normalized() const
{
	const float length = sqrt(x * x + y * y + z * z);
	return { x / length, y / length, z / length };
}

float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::operator-() const
{
	return { -x, -y, -z };
}
