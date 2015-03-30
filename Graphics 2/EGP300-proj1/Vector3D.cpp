#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D()
{
}


Vector3D::~Vector3D()
{
}

Vector3D::Vector3D(const Vector3D& rhs)
{
	X = rhs.X;
	Y = rhs.Y;
	Z = rhs.Z;
}

Vector3D::Vector3D(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3D::Vector3D(vec3 rhs)
{
	X = rhs.x;
	Y = rhs.y;
	Z = rhs.z;
}

Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
	return Vector3D(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
}

Vector3D Vector3D::operator+=(const Vector3D& rhs)
{
	this->X += rhs.X;
	this->Y += rhs.Y;
	this->Z += rhs.Z;
	return *this;
}

Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
	return Vector3D(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
}

Vector3D Vector3D::operator-=(const Vector3D& rhs)
{
	this->X -= rhs.X;
	this->Y -= rhs.Y;
	this->Z -= rhs.Z;
	return *this;
}

Vector3D Vector3D::operator*(const float& rhs) const
{
	return Vector3D(X * rhs, Y * rhs, Z * rhs);
}

Vector3D Vector3D::operator*=(const float& rhs)
{
	this->X *= rhs;
	this->Y *= rhs;
	this->Z *= rhs;
	return *this;
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

glm::vec3 Vector3D::toVec3()
{
	glm::vec3 ofTheJedi = vec3(X, Y, Z);
	return ofTheJedi;
}

float Vector3D::magnitude()
{
	return std::sqrt((X * X) + (Y * Y) + (Z * Z));
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

float Vector3D::dist(const Vector3D& a, const Vector3D& b)
{
	return glm::sqrt(distSquared(a, b));
}

float Vector3D::distSquared(const Vector3D& a, const Vector3D& b)
{
	return (pow((a.X - b.X), 2.0f) + pow((a.Y - b.Y), 2.0f) + pow((a.Z - b.Z), 2.0f));
}

float Vector3D::dist(const vec3& a, const vec3& b)
{
	return glm::sqrt(distSquared(a, b));
}

float Vector3D::distSquared(const vec3& a, const vec3& b)
{
	return (pow((a.x - b.x), 2.0f) + pow((a.y - b.y), 2.0f) + pow((a.z - b.z), 2.0f));
}