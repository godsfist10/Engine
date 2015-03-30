#include "AnchorSpringForceGen.h"


AnchorSpringForceGen::AnchorSpringForceGen()
{
}

AnchorSpringForceGen::AnchorSpringForceGen(Vector3D anchor, realNum springConstant, realNum restLength)
{
	mAnchorPoint = anchor;
	mSpringConstant = springConstant;
	mRestLength = restLength;
}

AnchorSpringForceGen::~AnchorSpringForceGen()
{
}

void AnchorSpringForceGen::updateForce(PhysicsObject* particle, double deltaTime)
{
	//find the directional of the force to apply
	Vector3D force = particle->getPos();
	force -= mAnchorPoint;
	// Calculate the magnitude of the force.
	realNum magnitude = force.magnitude();
	magnitude = std::abs(magnitude - mRestLength); //not sure why abs value. springs can compress as well.
	magnitude *= mSpringConstant;
	// Calculate the final force and apply it.
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}
