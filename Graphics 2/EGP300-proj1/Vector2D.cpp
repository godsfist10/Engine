#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D()
{
	X = 0;
	Y = 0;
}

Vector2D::~Vector2D(){}

Vector2D::Vector2D(float x, float y)
{
	X = x;
	Y = y;
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const
{
	return Vector2D(X + rhs.X, Y + rhs.Y);
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const
{
	return Vector2D(X - rhs.X, Y - rhs.Y);
}

Vector2D Vector2D::operator*(const float& rhs) const
{
	return Vector2D(X * rhs, Y * rhs);
}

Vector2D Vector2D::operator/(const float& rhs) const
{
	return Vector2D(X / rhs, Y / rhs);
}

bool Vector2D::operator==(const Vector2D& rhs) const
{
	if (X == rhs.X && Y == rhs.Y)
		return true;

	return false;
}

bool Vector2D::operator!=(const Vector2D& rhs) const
{
	if (X == rhs.X && Y == rhs.Y)
		return false;

	return true;
}

float Vector2D::magnitude()
{
	return sqrt((X * X) + (Y * Y));
}

float Vector2D::magnitudeSquared()
{
	return (X * X) + (Y * Y);
}

void Vector2D::normalize()
{
	Vector2D tempVec = normalized();
	X = tempVec.X;
	Y = tempVec.Y;
}

Vector2D Vector2D::normalized()
{
	return *this / magnitude();
}

float Vector2D::dotProduct(const Vector2D& rhs) const
{
	return (X * rhs.X + Y * rhs.Y);
}