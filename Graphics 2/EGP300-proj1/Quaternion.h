#ifndef QUATERNION_H
#define QUATERNION_H


class Vector3D;
class Matrix;

class Quaternion
{
public:
	float R;
	float I;
	float J;
	float K;
public:
	Quaternion();
	Quaternion(float r, float i, float j, float k);
	Quaternion(Vector3D rhs);
	Quaternion(const Quaternion&);
	virtual ~Quaternion();

	void Normalize();
	Quaternion* Normalized();

	Quaternion operator*(const Quaternion&);
	void operator*=(const Quaternion&);

	void RotateByVector(const Vector3D& rhs);
	void AddScaledVector(const Vector3D& rhs, float scale);

	Matrix* getRotationMatrix();
};

#endif // QUATERNION_H