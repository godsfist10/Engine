#pragma once

#include "ForceGenerator.h"
#include "Vector3D.h"

const double GRAVITY_CONST = 6.67384 * (pow(10, -11));

class GravityGenerator : public ForceGenerator
{
public:
	GravityGenerator();
	~GravityGenerator();

	virtual void update();

	inline void setSourceObject(PhysicsObject* obj) { mSourceObject = obj; }
	inline PhysicsObject* getSourceObject() { return mSourceObject; }

protected:

	PhysicsObject* mSourceObject;
	virtual void addForce(PhysicsObject* obj);

};

