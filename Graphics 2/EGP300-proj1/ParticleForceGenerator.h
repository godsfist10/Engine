#pragma once

#include "PhysicsObject.h"

class ParticleForceGenerator
{
public:
	ParticleForceGenerator();
	virtual ~ParticleForceGenerator();

	virtual void updateForce(PhysicsObject* particle, double deltaTime) = 0;

};

