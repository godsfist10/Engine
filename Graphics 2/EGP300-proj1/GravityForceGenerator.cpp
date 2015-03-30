#include "GravityForceGenerator.h"


GravityForceGenerator::GravityForceGenerator(const Vector3D &gravity)
{
	mGravity = gravity;
}


GravityForceGenerator::~GravityForceGenerator()
{
}

void GravityForceGenerator::updateForce(PhysicsObject* particle, double deltaTime)
{
	// Check that we do not have infinite mass.
	if (!particle->hasFiniteMass()) return;
	// Apply the mass-scaled force to the particle.
	particle->addForce(mGravity * (float)particle->getMass());

}