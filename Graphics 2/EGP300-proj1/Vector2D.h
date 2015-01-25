#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(float x, float y);
	~Vector2D();

	float X;
	float Y;

	Vector2D operator+(const Vector2D&) const;
	Vector2D operator-(const Vector2D&) const;
	Vector2D operator*(const float&) const;
	Vector2D operator/(const float&) const;
	bool operator==(const Vector2D&) const;
	bool operator!=(const Vector2D&) const;

	float magnitude();
	float magnitudeSquared();
	
	void normalize();
	Vector2D normalized();

	float dotProduct(const Vector2D&) const;
};

