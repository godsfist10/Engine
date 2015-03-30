#pragma once
#include "ParticleForceGenerator.h"
class ElasticForceGen :
	public ParticleForceGenerator
{

protected:
	PhysicsObject* mOther;
	/** Holds the spring constant. */
	realNum mSpringConstant;
	/** Holds the rest length of the spring. */
	realNum mRestLength;

public:
	ElasticForceGen();
	ElasticForceGen(PhysicsObject* other, realNum springConstant, realNum restLength);
	virtual ~ElasticForceGen();
	virtual void updateForce(PhysicsObject* particle, double deltaTime);
};

