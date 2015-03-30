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

	inline Vector3D getVelocity() { return mVelocity; }
	inline void setVelocity(Vector3D vel) { mVelocity = vel; }
	inline void modifyVelocity(Vector3D vel) { mVelocity += vel; }

	inline Vector3D getAcceleration() { return mAcceleration; }
	inline void setAcceleration(Vector3D val) { mAcceleration = val; }
	inline void modifyAcceleration(Vector3D val) { mAcceleration += val; }

	inline Vector3D getForce() { return mForce; }
	inline void clearForceAccumulation() { mForce = Vector3D(0, 0, 0); }

	inline double getMass() { return mMass; }
	inline void setMass(double mass) { mMass = (realNum)mass; mInverseMass = 1.0f / (realNum)mass; }

	inline void addForce(Vector3D force) { mForce += force; }

	inline bool hasFiniteMass() { return !infiniteMass; }

protected:

	virtual void physicsUpdate(double deltaTime);

	Vector3D mVelocity;
	Vector3D mAcceleration;
	Vector3D mForce;
	realNum mMass;
	realNum mInverseMass;
	realNum mDampeningVal;
	bool infiniteMass;
};

