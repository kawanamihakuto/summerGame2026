#include "Quaternion.h"
#include"Engine/Core/Precompiled.h"
Quaternion::Quaternion() :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w) :
	x(x),
	y(y),
	z(z),
	w(w)
{
}

Quaternion Quaternion::Identity()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

float Quaternion::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

Quaternion Quaternion::Normalize() const
{
	float length = Length();

	if (length <= 0.00001f)
	{
		return Identity();
	}

	float inv = 1.0f / length;

	return Quaternion(x * inv, y * inv, z * inv, w * inv);
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::Inverse() const
{
	float lengthSq = x * x + y * y + z * z + w * w;

	if (lengthSq <= 0.00001f)
	{
		return Identity();
	}

	Quaternion conjugate = Conjugate();

	float inv = 1.0f / lengthSq;

	return Quaternion(conjugate.x * inv,
					  conjugate.y * inv,
				 	  conjugate.z * inv,
					  conjugate.w * inv);
}

Vector3 Quaternion::Rotate(const Vector3& vec) const
{
	return Vector3();
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return Quaternion(
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y - x * q.z + y * q.w + z * q.x,
		w * q.z + x * q.y - y * q.x + z * q.w,
		w * q.w - x * q.x - y * q.y - z * q.z
	);
}
