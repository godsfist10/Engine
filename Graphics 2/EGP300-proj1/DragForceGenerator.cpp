#include "DragForceGenerator.h"

DragForceGenerator::DragForceGenerator(realNum dragCoefficient)
{
	dragCoef = dragCoefficient;
	dragCoefSquared = dragCoefficient * dragCoefficient;
}

DragForceGenerator::~DragForceGenerator()
{
}

void DragForceGenerator::updateForce(PhysicsObject* particle, double deltaTime)
{
	Vector3D force = particle->getVelocity();
	// Calculate the total drag coefficient.
	realNum dragCoeff = force.magnitude();
	
	dragCoeff = dragCoef * dragCoeff + dragCoefSquared * dragCoeff * dragCoeff;
	// Calculate the final force and apply it.
	force.normalize();
	
	if (dragCoeff > 0.0f)
	{
		force = force * -dragCoeff;
		particle->addForce(force);
	}
	

}