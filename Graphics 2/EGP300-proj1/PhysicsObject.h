#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

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
	virtual void resetObject();

	void init(Vector3D pos = Vector3D::ZERO, Vector3D vel = Vector3D::ZERO, Vector3D acc = Vector3D::ZERO);

	//wooooooooo adaptability. you want vector3D? thats good! you want glm::vec3? THATS GOOD TOO ILL MAKE IT WORK
	inline Vector3D getVelocity() { return mVelocity; }
	inline void setVelocity(Vector3D vel) { mVelocity = vel; }
	inline void setVelocity(float x, float y, float z) { setVelocity(Vector3D(x, y, z)); }
	inline void setVelocity(glm::vec3 vel) { setVelocity(Vector3D(vel)); }
	inline void modifyVelocity(Vector3D vel) { mVelocity += vel; }

	inline Vector3D getAcceleration() { return mAcceleration; }
	inline void setAcceleration(Vector3D val) { mAcceleration = val; }
	inline void setAcceleration(glm::vec3 val) { mAcceleration = val; }
	inline void setAcceleration(float x, float y, float z) { mAcceleration = Vector3D(x, y, z); }
	inline void modifyAcceleration(Vector3D val) { mAcceleration += val; }

	inline Vector3D getForce() { return mForce; }
	inline void clearForceAccumulation() { mForce = Vector3D(0, 0, 0); }

	inline double getMass() { return mMass; }
	inline double getInverseMass() { return mInverseMass; }
	inline void setMass(double mass) { mMass = (realNum)mass; mInverseMass = 1.0f / (realNum)mass; }

	inline void addForce(Vector3D force) { mForce += force; }
	inline void addForce(glm::vec3 force) { mForce += force; }

	inline bool hasFiniteMass() { return !infiniteMass; }
	inline float getRestitution() { return mRestitution; };
	inline void setRestitution(realNum val) { mRestitution = val; }
protected:

	virtual void physicsUpdate(double deltaTime);

	Vector3D mVelocity;
	Vector3D mAcceleration;
	Vector3D mForce;

	realNum mMass;
	realNum mInverseMass;
	realNum mDampeningVal;
	bool infiniteMass;
	realNum mRestitution;

	Vector3D startVel;
	Vector3D startAcc;
};

#endif // PHYSICSOBJECT_H
