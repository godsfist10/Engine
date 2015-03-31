#pragma once
#include "ParticleForceGenerator.h"
#include "Vector3D.h"

const double GRAVITY_CONST = 3.965f * pow(10.0f, -14.0f);

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

