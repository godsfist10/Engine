#pragma once
class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	~Vector3D();

	float X;
	float Y;
	float Z;

	Vector3D operator+(const Vector3D&) const;
	Vector3D operator-(const Vector3D&) const;
	Vector3D operator*(const float&) const;
	Vector3D operator/(const float&) const;
	bool operator==(const Vector3D&) const;
	bool operator!=(const Vector3D&) const;

	float magnitude();
	float magnitudeSquared();

	void normalize();
	Vector3D normalized();

	Vector3D cross(const Vector3D&) const;
	float dot(const Vector3D&) const;
};

