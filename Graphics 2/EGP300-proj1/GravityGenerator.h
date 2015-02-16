#pragma once

#include "ForceGenerator.h"
#include "Vector3D.h"

const double GRAVITY_CONST = 3.965f * pow(10.0f, -14.0f);

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

