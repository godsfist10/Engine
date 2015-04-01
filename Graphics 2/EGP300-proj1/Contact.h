#pragma once

#include "PhysicsObject.h"

//contact class that is created when a collision/collisionlike occurs

class Contact
{
public:
	
	Contact(float restitution, float penetration, Vector3D contactNormal, PhysicsObject* contactOne, PhysicsObject* contactTwo = nullptr); //contact 2 can be null for scenery collisions
	~Contact();

	void Resolve(float deltaTime);

private:
	PhysicsObject* mpContactOne;
	PhysicsObject* mpContactTwo;
	float mRestitution;
	float mPenetration;
	Vector3D mContactNormal;

	void resolveVelocity(realNum deltaTime);
	void resolveInterpenetration(realNum deltaTime);
	float calculateSeparatingVelocity();

};
