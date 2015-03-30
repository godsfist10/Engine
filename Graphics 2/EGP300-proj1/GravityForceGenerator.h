#pragma once
#include "ParticleForceGenerator.h"
#include "Vector3D.h"

class GravityForceGenerator :
	public ParticleForceGenerator
{
public:
	GravityForceGenerator(const Vector3D &gravity);
	virtual ~GravityForceGenerator();
	virtual void updateForce(PhysicsObject* particle, double deltaTime);

protected:
	Vector3D mGravity;
};

