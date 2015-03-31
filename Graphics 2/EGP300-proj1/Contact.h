#pragma once

#include "PhysicsObject.h"

//contact class that is created when a collision/collisionlike occurs

class Contact
{
private:
	PhysicsObject* mp_ContactOne;
	PhysicsObject* mp_ContactTwo;

	float m_Restitution;
	float m_Penetration;

	Vector3D m_ContactNormal;

private:
	void resolveVelocity(realNum deltaTime);
	void resolveInterpenetration(realNum deltaTime);
	float calculateSeparatingVelocity();

public:
	
	Contact(float restitution, float penetration, Vector3D contactNormal, PhysicsObject* contactOne, PhysicsObject* contactTwo = nullptr); //contact 2 can be null for scenery collisions
	~Contact();

	void Resolve(float deltaTime);
};
