#include "PhysicsObject.h"


PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap)
:Object(modelsMap)
{
	mVelocity = vec3(0, 0, 0); 
	mAcceleration = vec3(0, 0, 0);
	mForce = vec3(0, 0, 0);
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap, const double& mass)
	:Object(modelsMap)
{
	mVelocity = vec3(0, 0, 0);
	mAcceleration = vec3(0, 0, 0);
	mForce = vec3(0, 0, 0);
	mMass = (realNum)mass;
	mInverseMass = (realNum)1.0 / mMass;
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::update()
{
	physicsUpdate();
}

void PhysicsObject::physicsUpdate()
{
	setAcceleration(mForce * mInverseMass);
	mForce = vec3(0, 0, 0);
	modifyVelocity(mAcceleration);
	modifyPos(mVelocity);
}

