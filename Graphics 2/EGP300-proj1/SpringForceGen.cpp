#include "SpringForceGen.h"


SpringForceGen::SpringForceGen()
{
}

SpringForceGen::SpringForceGen(PhysicsObject* other, realNum springConstant, realNum restLength)
{
	mOther = other;
	mSpringConstant = springConstant;
	mRestLength = restLength;
}

SpringForceGen::~SpringForceGen()
{
}

void SpringForceGen::updateForce(PhysicsObject* particle, double deltaTime)
{
	//find the directional of the force to apply
	Vector3D force = particle->getPos();
	Vector3D pos = mOther->getPos();
	force -= pos;
	// Calculate the magnitude of the force.
	realNum magnitude = force.magnitude();
	magnitude = mSpringConstant * (magnitude - mRestLength); //not sure why abs value in the book. springs can compress as well.
	// Calculate the final force and apply it.
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}

