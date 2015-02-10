#include "GravityGenerator.h"


GravityGenerator::GravityGenerator()
{
}


GravityGenerator::~GravityGenerator()
{
}

void GravityGenerator::update()
{
	for (std::vector<PhysicsObject*>::iterator it = mRegistry.begin(); it != mRegistry.end(); ++it)
	{
		addForce(*it);
	}
}

void GravityGenerator::addForce(PhysicsObject* obj )
{
	float distanceSQ = Vector3D::distSquared(mSourceObject->getPos(), obj->getPos());
	
	Vector3D direction = obj->getPos() - mSourceObject->getPos();
	direction.normalize();

	double force = ((GRAVITY_CONST * mSourceObject->getMass() * obj->getMass()) / (double)distanceSQ);

	Vector3D gravity = direction * (float)force;
	vec3 forceToAdd = vec3(gravity.X, gravity.Y, gravity.Z);
	//mSourceObject->addForce(forceToAdd);
	obj->addForce(-forceToAdd);

}