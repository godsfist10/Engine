#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
	mDampeningVal = .999999999999f;
	infiniteMass = false;
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap)
:Object(modelsMap)
{
	mVelocity = vec3(0, 0, 0); 
	mAcceleration = vec3(0, 0, 0);
	mForce = vec3(0, 0, 0);
	mDampeningVal = .999999999999f;
	infiniteMass = false;
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap, const double& mass)
	:Object(modelsMap)
{
	mVelocity = vec3(0, 0, 0);
	mAcceleration = vec3(0, 0, 0);
	mForce = vec3(0, 0, 0);
	mMass = (realNum)mass;
	mInverseMass = (realNum)1.0 / mMass;
	mDampeningVal = .99999999f;
	infiniteMass = false;
}

PhysicsObject::~PhysicsObject()
{
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

