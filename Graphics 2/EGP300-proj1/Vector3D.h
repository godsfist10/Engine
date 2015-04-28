#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(vec3 rhs);
	Vector3D(const Vector3D& rhs);
	~Vector3D();

	float X;
	float Y;
	float Z;

	static const Vector3D ZERO;

	Vector3D operator+(const Vector3D&) const;
	Vector3D operator+=(const Vector3D&);
	Vector3D operator-(const Vector3D&) const;
	Vector3D operator-=(const Vector3D&);
	Vector3D operator*(const float&) const;
	Vector3D operator*=(const float&);
	Vector3D operator/(const float&) const;
	bool operator==(const Vector3D&) const;
	bool operator!=(const Vector3D&) const;
	glm::vec3 toVec3();

	float magnitude();
	float magnitudeSquared();

	void normalize();
	Vector3D normalized();

	Vector3D cross(const Vector3D&) const;
	float dot(const Vector3D&) const;

	static float dist(const Vector3D& a, const Vector3D& b);
	static float dist(const vec3& a, const vec3& b);
	static float distSquared(const Vector3D& a, const Vector3D& b);
	static float distSquared(const vec3& a, const vec3& b);
};

#endif //VECTOR3D_H