#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D()
{
}


Vector3D::~Vector3D()
{
}

Vector3D::Vector3D(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
	return Vector3D(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
}

Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
	return Vector3D(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
}

Vector3D Vector3D::operator*(const float& rhs) const
{
	return Vector3D(X * rhs, Y * rhs, Z * rhs);
}

Vector3D Vector3D::operator/(const float& rhs) const
{
	return Vector3D(X / rhs, Y / rhs, Z / rhs);
}

bool Vector3D::operator==(const Vector3D& rhs) const
{
	if (X == rhs.X && Y == rhs.Y && Z == rhs.Z)
		return true;
	return false;
}

bool Vector3D::operator!=(const Vector3D& rhs) const
{
	if (X == rhs.X && Y == rhs.Y && Z == rhs.Z)
		return false;
	return true;
}

float Vector3D::magnitude()
{
	return sqrt((X * X) + (Y * Y) + (Z * Z));
}

float Vector3D::magnitudeSquared()
{
	return (X * X) + (Y * Y) + (Z * Z);
}

void Vector3D::normalize()
{
	Vector3D tempVec = normalized();
	X = tempVec.X;
	Y = tempVec.Y;
	Z = tempVec.Z;
}

Vector3D Vector3D::normalized()
{
	return *this / magnitude();
}

Vector3D Vector3D::cross(const Vector3D& rhs) const
{
	return Vector3D(Y * rhs.Z - Z * rhs.Y, Z * rhs.X - X * rhs.Z, X * rhs.Y - Y * rhs.X);
}

float Vector3D::dot(const Vector3D& rhs) const
{
	return (X * rhs.X + Y * rhs.Y + Z * rhs.Z);
}