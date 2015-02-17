#include "PhysicsObject.h"


PhysicsObject::PhysicsObject()
{
	mDampeningVal = .999999999999f;
	mSystemTimeMult = 86400.0;
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap)
:Object(modelsMap)
{
	mVelocity = vec3(0, 0, 0); 
	mAcceleration = vec3(0, 0, 0);
	mForce = vec3(0, 0, 0);
	mDampeningVal = .999999999999f;
	mSystemTimeMult = 86400.0;
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
	mSystemTimeMult = 86400.0;
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
	double systemTime = deltaTime;
	systemTime *= mSystemTimeMult;
	// day per frame
		//systemTime *= 86400.0; 
	// week per frame
		//systemTime *= 604800.0;
	// month per frame
		//systemTime *= 2628000.0;
	// year per frame
		//systemTime *= 31536000.0;

	modifyPos(mVelocity * (float)systemTime);
	setAcceleration(mForce * mInverseMass);
	mForce = vec3(0, 0, 0);
	modifyVelocity(mAcceleration * (float)systemTime);
	mVelocity *= mDampeningVal;
	
}

