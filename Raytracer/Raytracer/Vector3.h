#pragma once

// TODO: add padding to align the class to 16 bytes
class Vector3
{
public:
	float x, y, z;

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator*(float scalar) const;
	float operator*(const Vector3& other) const;
	Vector3 cross(const Vector3& other) const;
	void normalize();
	Vector3 normalized() const;
	float length() const;
	Vector3 operator-() const;
};