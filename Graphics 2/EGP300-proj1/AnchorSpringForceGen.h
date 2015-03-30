#pragma once
#include "ParticleForceGenerator.h"
class AnchorSpringForceGen : public ParticleForceGenerator
{
protected:
	Vector3D mAnchorPoint;
	/** Holds the spring constant. */
	realNum mSpringConstant;
	/** Holds the rest length of the spring. */
	realNum mRestLength;


public:
	AnchorSpringForceGen();
	AnchorSpringForceGen(Vector3D anchor, realNum springConstant, realNum restLength);
	virtual ~AnchorSpringForceGen();

	virtual void updateForce(PhysicsObject* particle, double deltaTime);
};

