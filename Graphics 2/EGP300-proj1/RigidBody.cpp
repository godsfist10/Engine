#include "RigidBody.h"

RigidBody::RigidBody()
{
	
	m_TransformationMatrix = Matrix(4, 4);
	m_InverseInertiaTensor = Matrix(3, 3);
	m_InverseInertiaTensorWorld = Matrix(3, 3);
	
}

RigidBody::~RigidBody()
{
}

void RigidBody::CalculateDerivedData()
{
	mOrientation.Normalize();

	calculateTransformMatrix(m_TransformationMatrix, mPos, mOrientation);

	transformInertiaTensor(m_InverseInertiaTensorWorld, mOrientation, m_InverseInertiaTensor, m_TransformationMatrix);
}

void RigidBody::calculateTransformMatrix(Matrix &transformMatrix, const Vector3D &position, const Quaternion &orientation)
{
	float val1 = 1 - 2 * orientation.J * orientation.J - 2 * orientation.K * orientation.K;
	transformMatrix.Set(0, 0, val1);
	float val2 = 2 * orientation.I * orientation.J - 2 * orientation.R * orientation.K;
	transformMatrix.Set(0, 1, val2);
	float val3 = 2 * orientation.I * orientation.K + 2 * orientation.R * orientation.J;
	transformMatrix.Set(0, 2, val3);
	float val4 = position.X;
	transformMatrix.Set(0, 3, val4);

	float val5 = 2 * orientation.I * orientation.J + 2 * orientation.R * orientation.K;
	transformMatrix.Set(1, 0, val5);
	float val6 = 1 - 2 * orientation.I * orientation.I - 2 * orientation.K * orientation.K;
	transformMatrix.Set(1, 1, val6);
	float val7 = 2 * orientation.J * orientation.K - 2 * orientation.R * orientation.I;
	transformMatrix.Set(1, 2, val7);
	float val8 = position.Y;
	transformMatrix.Set(1, 3, val8);

	float val9 = 2 * orientation.I * orientation.K - 2 * orientation.R * orientation.J;
	transformMatrix.Set(2, 0, val9);
	float val10 = 2 * orientation.J * orientation.K + 2 * orientation.R * orientation.I;
	transformMatrix.Set(2, 1, val10);
	float val11 = 1 - 2 * orientation.I * orientation.I - 2 * orientation.J * orientation.J;
	transformMatrix.Set(2, 2, val11);
	float val12 = position.Z;
	transformMatrix.Set(2, 3, val12);
}

void RigidBody::transformInertiaTensor(Matrix &iitWorld, const Quaternion &quaternion, const Matrix &iitBody, const Matrix &rotationMatrix)
{
	float t4 = rotationMatrix.Get(0) * iitBody.Get(0) + rotationMatrix.Get(1) * iitBody.Get(3) + rotationMatrix.Get(2) * iitBody.Get(6);
	float t9 = rotationMatrix.Get(0) * iitBody.Get(1) + rotationMatrix.Get(1) * iitBody.Get(4) + rotationMatrix.Get(2) * iitBody.Get(7);
	float t14 = rotationMatrix.Get(0) * iitBody.Get(2) + rotationMatrix.Get(1) * iitBody.Get(5) + rotationMatrix.Get(2) * iitBody.Get(8);
	float t28 = rotationMatrix.Get(4) * iitBody.Get(0) + rotationMatrix.Get(5) * iitBody.Get(3) + rotationMatrix.Get(6) * iitBody.Get(6);
	float t33 = rotationMatrix.Get(4) * iitBody.Get(1) + rotationMatrix.Get(5) * iitBody.Get(4) + rotationMatrix.Get(6) * iitBody.Get(7);
	float t38 = rotationMatrix.Get(4) * iitBody.Get(2) + rotationMatrix.Get(5) * iitBody.Get(5) + rotationMatrix.Get(6) * iitBody.Get(8);
	float t52 = rotationMatrix.Get(8) * iitBody.Get(0) + rotationMatrix.Get(9) * iitBody.Get(3) + rotationMatrix.Get(10) * iitBody.Get(6);
	float t57 = rotationMatrix.Get(8) * iitBody.Get(1) + rotationMatrix.Get(9) * iitBody.Get(4) + rotationMatrix.Get(10) * iitBody.Get(7);
	float t62 = rotationMatrix.Get(8) * iitBody.Get(2) + rotationMatrix.Get(9) * iitBody.Get(5) + rotationMatrix.Get(10) * iitBody.Get(8);

	float val0 = t4 * rotationMatrix.Get(0) + t9 * rotationMatrix.Get(1) + t14 * rotationMatrix.Get(2);
	iitWorld.Set(0, val0);
	float val1 = t4 * rotationMatrix.Get(4) + t9 * rotationMatrix.Get(5) + t14 * rotationMatrix.Get(6);
	iitWorld.Set(1, val1);
	float val2 = t4 * rotationMatrix.Get(8) + t9 * rotationMatrix.Get(9) + t14 * rotationMatrix.Get(10);
	iitWorld.Set(2, val2);
	float val3 = t28 * rotationMatrix.Get(0) + t33 * rotationMatrix.Get(1) + t38 * rotationMatrix.Get(2);
	iitWorld.Set(3, val3);
	float val4 = t28 * rotationMatrix.Get(4) + t33 * rotationMatrix.Get(5) + t38 * rotationMatrix.Get(6);
	iitWorld.Set(4, val4);
	float val5 = t28 * rotationMatrix.Get(8) + t33 * rotationMatrix.Get(9) + t38 * rotationMatrix.Get(10);
	iitWorld.Set(5, val5);
	float val6 = t52 * rotationMatrix.Get(0) + t57 * rotationMatrix.Get(1) + t62 * rotationMatrix.Get(2);
	iitWorld.Set(6, val6);
	float val7 = t52 * rotationMatrix.Get(4) + t57 * rotationMatrix.Get(5) + t62 * rotationMatrix.Get(6);
	iitWorld.Set(7, val7);
	float val8 = t52 * rotationMatrix.Get(8) + t57 * rotationMatrix.Get(9) + t62 * rotationMatrix.Get(10);
	iitWorld.Set(8, val8);
}

Vector3D RigidBody::GetPointInWorldSpace(const Vector3D point)
{
	return m_TransformationMatrix.Transform(point);
}

void RigidBody::SetIntertiaTensor(const Matrix &intertiaTensor)
{
	m_InverseInertiaTensor = intertiaTensor.Inverse();
}

void RigidBody::AddForce(const Vector3D &force)
{
	mForce += force;
	mIsAwake = true;
}

void RigidBody::ClearAccumulators()
{
	mForce = Vector3D::ZERO;
	mTorqueAccum = Vector3D::ZERO;
}

void RigidBody::Integrate(float duration)
{
	mLastFrameAcceleration = mAcceleration;
	mLastFrameAcceleration += (mForce * mInverseMass);

	Vector3D angularAcceleration = m_InverseInertiaTensorWorld.Transform(mTorqueAccum);

	mVelocity += (mLastFrameAcceleration * duration);

	mRotation += (angularAcceleration * duration).toVec3();

	mVelocity *= (pow(m_LinearDampening, duration));
	mRotation *= (pow(m_AngularDampening, duration));

	mPos += (mVelocity * duration).toVec3();

	mOrientation.AddScaledVector(mRotation, duration);

	CalculateDerivedData();

	ClearAccumulators();
}

void RigidBody::Initialize(float mass, Vector3D initialPosition, Vector3D initialVelocity, Vector3D initialAcceleration, Vector3D initialRotation, float dampening, float angularDampening)
{

	m_LinearDampening = dampening;
	m_AngularDampening = angularDampening;

	init(initialPosition, initialVelocity, initialAcceleration);
	setMass(mass);
	mLastFrameAcceleration = initialAcceleration;
	mRotation = initialRotation.toVec3();

	mIsAwake = false;

	m_TransformationMatrix = Matrix(4, 4);
}

void RigidBody::Reset()
{
	resetObject();
	mLastFrameAcceleration = Vector3D::ZERO;
	mRotation = vec3(0, 0, 0);
}

void RigidBody::AddForceAtPoint(const Vector3D &force, const Vector3D &point)
{
	Vector3D worldPoint = point;
	worldPoint -= mPos;

	mForce += force;
	mTorqueAccum += worldPoint.cross(force);

	mIsAwake = true;
}

void RigidBody::AddForceAtBodyPoint(const Vector3D &force, const Vector3D &point)
{
	Vector3D worldPoint = GetPointInWorldSpace(point);
	AddForceAtPoint(force, worldPoint);
	mIsAwake = true;
}

void RigidBody::GetInverseInertiaTensorWorld(Matrix& inverseInertiaTensor) const
{
	inverseInertiaTensor = m_InverseInertiaTensorWorld;
}

void RigidBody::AddVelocity(const Vector3D& deltaVelocity)
{
	mVelocity += deltaVelocity;
}

void RigidBody::AddRotation(const Vector3D& deltaRotation)
{
	Vector3D temp = Vector3D(deltaRotation);
	mRotation += temp.toVec3();
}