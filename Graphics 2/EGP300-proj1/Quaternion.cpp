#include "Quaternion.h"
#include <math.h>
#include "Vector3D.h"
#include "Matrix.h"

Quaternion::Quaternion()
{
	R = 0;
	I = 0;
	J = 0;
	K = 0;
}

Quaternion::Quaternion(float r, float i, float j, float k)
{
	R = r;
	I = i;
	J = j;
	K = k;
}

Quaternion::Quaternion(Vector3D rhs)
{
	R = 0;
	I = rhs.X;
	J = rhs.Y;
	K = rhs.Z;
}

Quaternion::Quaternion(const Quaternion& rhs)
{
	R = rhs.R;
	I = rhs.I;
	J = rhs.J;
	K = rhs.K;
}

Quaternion::~Quaternion()
{
}

void Quaternion::Normalize()
{
	float length = R*R + I*I + J*J + K*K;

	if (length == 0)
	{
		R = 1;
		return;
	}

	length = 1.0f / sqrtf(length);

	R *= length;
	I *= length;
	J *= length;
	K *= length;
}

Quaternion* Quaternion::Normalized()
{
	float length = R*R + I*I + J*J + K*K;

	if (length == 0)
	{
		R = 1;
		return this;
	}

	length = 1.0f / sqrtf(length);

	R *= length;
	I *= length;
	J *= length;
	K *= length;

	return this; 
}

Quaternion Quaternion::operator*(const Quaternion& rhs)
{
	Quaternion q = Quaternion((*this));
	q *= rhs;
	return q;
}

void Quaternion::operator*=(const Quaternion& rhs)
{
	Quaternion q = *this;

	R = q.R * rhs.R - q.I * rhs.I - q.J * rhs.J - q.K * rhs.K;
	I = q.R * rhs.I + q.I * rhs.R + q.J * rhs.K - q.K * rhs.J;
	J = q.R * rhs.J + q.J * rhs.R + q.K * rhs.I - q.I * rhs.K;
	K = q.R * rhs.K + q.K * rhs.R + q.I * rhs.J - q.J * rhs.I;
}

void Quaternion::RotateByVector(const Vector3D& rhs)
{
	Quaternion q = Quaternion(rhs);

	(*this) *= q;
}

void Quaternion::AddScaledVector(const Vector3D& rhs, float scale)
{
	Vector3D scaledVector = Vector3D(rhs);
	scaledVector *= scale;
	Quaternion q = Quaternion(scaledVector);

	q *= (*this);
	R += q.R * (0.5f);
	I += q.I * (0.5f);
	J += q.J * (0.5f);
	K += q.K * (0.5f);
}

Matrix* Quaternion::getRotationMatrix()
{
	Quaternion q = *this;

	float vals[9] =		{ 1.0f - (2.0f * q.J * q.J + 2.0f * q.K * q.K)	, (2.0f * q.I * q.J + 2.0f * q.K *q.R)			, (2.0f * q.I * q.K - 2.0f * q.J*q.R),
						(2.0f*q.I*q.J - 2.0f*q.K*q.R)					, (1.0f - (2.0f * q.I * q.I + 2.0f * q.K * q.K)), (2.0f * q.J * q.K + 2.0f * q.I * q.R),
						(2.0f * q.I * q.K - 2.0f * q.J*q.R)				, (2.0f * q.J * q.K + 2.0f * q.I * q.R)			, (1.0f - (2.0f * q.I * q.I + 2.0f * q.J * q.J))};

	Matrix* matrix = new Matrix(3, 3, vals);
	
	return matrix;
}