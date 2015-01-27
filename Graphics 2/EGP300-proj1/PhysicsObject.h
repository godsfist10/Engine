#pragma once
#include "Object.h"

using namespace std;
using namespace glm;
using namespace Arc;

class PhysicsObject : public Object
{
public:
	PhysicsObject();
	PhysicsObject(const Map<string, Model*> &modelsMap);
	~PhysicsObject();

	/*inline PhysicsObject(const PhysicsObject& rhs)
		: mModelsMap(rhs.mModelsMap),
		mPos(rhs.mPos),
		mRotation(rhs.mRotation),
		mScale(rhs.mScale),
		mIsPrefab(rhs.mIsPrefab)
	{ 
		m_Velocity = vec3(0, 0, 0);
		m_Acceleration = vec3(0, 0, 0);
	}*/

	virtual void update();

	inline vec3 getVelocity() { return m_Velocity; }
	inline void setVelocity(vec3 vel) { m_Velocity = vel; }
	inline void modifyVelocity(vec3 vel) { m_Velocity += vel; }

	inline vec3 getAcceleration() { return m_Acceleration; }
	inline void setAcceleration(vec3 val) { m_Acceleration = val; }
	inline void modifyAcceleration(vec3 val) { m_Acceleration += val; }

protected:

	virtual void physicsUpdate();

	vec3 m_Velocity;
	vec3 m_Acceleration;

};

