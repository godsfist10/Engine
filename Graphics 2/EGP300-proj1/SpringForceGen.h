#pragma once
#include "ParticleForceGenerator.h"
class SpringForceGen :
	public ParticleForceGenerator
{
protected:
	PhysicsObject* mOther;
	/** Holds the spring constant. */
	realNum mSpringConstant;
	/** Holds the rest length of the spring. */
	realNum mRestLength;

public:
	SpringForceGen();
	SpringForceGen(PhysicsObject* other, realNum springConstant, realNum restLength);
	virtual ~SpringForceGen();
	virtual void updateForce(PhysicsObject* particle, double deltaTime);
};

