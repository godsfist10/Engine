#pragma once
#include "ParticleForceGenerator.h"

class DragForceGenerator :
	public ParticleForceGenerator
{
protected:
	/** Holds the velocity drag coefficient. */
	realNum dragCoef;
	/** Holds the velocity squared drag coefficient. */
	realNum dragCoefSquared;

public:
	DragForceGenerator(realNum dragCoefficient);
	virtual ~DragForceGenerator();
	virtual void updateForce(PhysicsObject* particle, double deltaTime);

};

