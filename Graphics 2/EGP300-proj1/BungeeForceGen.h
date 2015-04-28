#pragma once
#include "ParticleForceGenerator.h"
class BungeeForceGen :
	public ParticleForceGenerator
{
protected:
	PhysicsObject* mOther;
	/** Holds the spring constant. */
	realNum mSpringConstant;
	/** Holds the rest length of the spring. */
	realNum mRestLength;

public:
	BungeeForceGen();
	BungeeForceGen(PhysicsObject* other, realNum springConstant, realNum restLength);
	virtual void updateForce(PhysicsObject *particle, double deltaTime);
	virtual ~BungeeForceGen();
};

