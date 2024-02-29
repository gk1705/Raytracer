#include "Quaternion.h"

#include <cmath>

Quaternion Quaternion::fromAxisAngle(const Vector3& axis, float angle)
{
	// for our left-handed coordinate system we need to invert the angle
	const float halfAngle = -angle * 0.5f;
	float s = std::sin(halfAngle);
	return { std::cos(halfAngle), axis.x * s, axis.y * s, axis.z * s };
}

Quaternion Quaternion::normalized() const
{
	const float length = std::sqrt(w * w + x * x + y * y + z * z);
	return { w / length, x / length, y / length, z / length };
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return {
		w * other.w - x * other.x - y * other.y - z * other.z,
		w * other.x + x * other.w + y * other.z - z * other.y,
		w * other.y - x * other.z + y * other.w + z * other.x,
		w * other.z + x * other.y - y * other.x + z * other.w
	};
}

Vector3 Quaternion::operator*(const Vector3& vec) const
{
	const Quaternion vecQuat(0, vec.x, vec.y, vec.z);
	const Quaternion conj(w, -x, -y, -z);

	// apply rotation: q * v * q^(-1)
	// first, multiply the quaternion by the vector (treated as quaternion).
	// then, the result is multiplied by the conjugate of the original quaternion.
	const Quaternion resultQuat = *this * vecQuat * conj;

	return Vector3(resultQuat.x, resultQuat.y, resultQuat.z);
}
