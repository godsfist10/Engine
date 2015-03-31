#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
	mDampeningVal = .999999999999f;
	infiniteMass = false;
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap)
:Object(modelsMap)
{
	mVelocity = Vector3D::ZERO;
	mAcceleration = Vector3D::ZERO;
	mForce = Vector3D::ZERO;
	mDampeningVal = .999999999999f;
	infiniteMass = false;
	mRestitution = .5f;

	startVel = Vector3D::ZERO;
	startAcc = Vector3D::ZERO;
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap, const double& mass)
	:Object(modelsMap)
{
	mVelocity = Vector3D::ZERO;
	mAcceleration = Vector3D::ZERO;
	mForce = Vector3D::ZERO;
	mMass = (realNum)mass;
	mInverseMass = (realNum)1.0 / mMass;
	mDampeningVal = .99999999f;
	infiniteMass = false;
	mRestitution = .5f;

	startVel = Vector3D::ZERO;
	startAcc = Vector3D::ZERO;
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::init(Vector3D pos, Vector3D vel, Vector3D acc)
{
	mPos = pos.toVec3();
	startPos = pos.toVec3();

	mVelocity = vel;
	startVel = vel;

	mAcceleration = acc;
	startAcc = acc;
}

void PhysicsObject::update(double deltaTime)
{
	physicsUpdate(deltaTime);
}

void PhysicsObject::physicsUpdate(double deltaTime)
{
	modifyPos(mVelocity.toVec3() *(float)deltaTime);
	setAcceleration(mForce * mInverseMass);
	modifyVelocity(mAcceleration * (float)deltaTime);
	mVelocity *= (float)mDampeningVal;
	clearForceAccumulation();
}

void PhysicsObject::resetObject()
{
	Object::resetObject();
	mVelocity = startVel;
	mAcceleration = startAcc;
}