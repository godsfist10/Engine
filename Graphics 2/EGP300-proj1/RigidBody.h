#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Quaternion.h"
#include "Matrix.h"
#include "PhysicsObject.h"
//#include "CollisionSphere.h"
class RigidBody : public PhysicsObject
{
public:
	RigidBody();
	~RigidBody();

	void CalculateDerivedData();
	void SetIntertiaTensor(const Matrix &intertiaTensor);
	void AddForce(const Vector3D &force);
	void ClearAccumulators();
	void Integrate(float duration);

	virtual void Initialize(float mass, Vector3D initialPosition, Vector3D initialVelocity = Vector3D::ZERO, Vector3D initialAcceleration = Vector3D::ZERO, Vector3D initialRotation = Vector3D::ZERO, float linearDampening = 1, float angularDampening = 1);
	void Reset();

	void AddForceAtPoint(const Vector3D &force, const Vector3D &point);
	void AddForceAtBodyPoint(const Vector3D &force, const Vector3D &point);

	inline bool GetAwake() { return mIsAwake; }
	inline void SetAwake() { mIsAwake = true; }

	void SetOrientation(Quaternion orientation) { mOrientation = orientation; };

	Quaternion GetOrientation() { return mOrientation; };

	Vector3D GetPointInWorldSpace(const Vector3D point);
	Matrix GetTransform() const { return m_TransformationMatrix; };
	Vector3D GetPosition() { return mPos; };
	Vector3D GetRotation() { return mRotation; };
	Vector3D GetLastFrameAcceleration() { return mLastFrameAcceleration; };
	//virtual CollisionSphere GetCollisionSphere() { return CollisionSphere(this, 1); };

	void GetInverseInertiaTensorWorld(Matrix& inverseInertiaTensor) const;

	void AddVelocity(const Vector3D& deltaVelocity);
	void AddRotation(const Vector3D& deltaRotation);

protected:
	float m_LinearDampening;
	float m_AngularDampening;

	Vector3D mLastFrameAcceleration;
	Vector3D mTorqueAccum;

	Quaternion mOrientation;

	Matrix m_TransformationMatrix;
	Matrix m_InverseInertiaTensor;
	Matrix m_InverseInertiaTensorWorld;

	bool mIsAwake;

	static void calculateTransformMatrix(Matrix &transformMatrix, const Vector3D &position, const Quaternion &orientation);
	static void transformInertiaTensor(Matrix &iitWorld, const Quaternion &quaternion, const Matrix &iitBody, const Matrix &rotationMatrix);
	
};
//=============================================================================
#endif // RIGIDBODY_H
