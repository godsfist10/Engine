#include "ElasticForceGen.h"


ElasticForceGen::ElasticForceGen()
{
}


ElasticForceGen::ElasticForceGen(PhysicsObject* other, realNum springConstant, realNum restLength)
{
	mOther = other;
	mSpringConstant = springConstant;
	mRestLength = restLength;
}

ElasticForceGen::~ElasticForceGen()
{
}

void ElasticForceGen::updateForce(PhysicsObject* particle, double deltaTime)
{
	//find the directional of the force to apply
	Vector3D force = particle->getPos();
	Vector3D pos = mOther->getPos();
	force -= pos;
	// Calculate the magnitude of the force.
	realNum magnitude = force.magnitude();

	if (magnitude <= mRestLength) return;

	magnitude = mSpringConstant * (magnitude - mRestLength); //not sure why abs value. springs can compress as well.
	// Calculate the final force and apply it.
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}
