#pragma once
#include "Object.h"

using namespace std;
using namespace glm;
using namespace Arc;

typedef float realNum;

class PhysicsObject : public Object
{
public:
	PhysicsObject();
	PhysicsObject(const Map<string, Model*> &modelsMap);
	PhysicsObject(const Map<string, Model*> &modelsMap, const double& mass);
	~PhysicsObject();

	virtual void update(double deltaTime);

	inline vec3 getVelocity() { return mVelocity; }
	inline void setVelocity(vec3 vel) { mVelocity = vel; }
	inline void modifyVelocity(vec3 vel) { mVelocity += vel; }

	inline vec3 getAcceleration() { return mAcceleration; }
	inline void setAcceleration(vec3 val) { mAcceleration = val; }
	inline void modifyAcceleration(vec3 val) { mAcceleration += val; }

	inline void setSystemTimeMult(double val) { mSystemTimeMult = val; }
	inline double getSystemTimeMult() { return mSystemTimeMult; }
	inline void modSystemTimeMult(double val) { mSystemTimeMult += val; }

	inline vec3 getForce() { return mForce; }

	inline double getMass() { return mMass; }
	inline void setMass(double mass) { mMass = (realNum)mass; mInverseMass = 1.0f / (realNum)mass; }

	inline void addForce(vec3 force) { mForce += force; }

protected:

	virtual void physicsUpdate(double deltaTime);

	vec3 mVelocity;
	vec3 mAcceleration;
	vec3 mForce;
	realNum mMass;
	realNum mInverseMass;
	realNum mDampeningVal;
	double mSystemTimeMult;
};

