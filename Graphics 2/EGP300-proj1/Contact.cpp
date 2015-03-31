#include "Contact.h"
#include "PhysicsObject.h"

Contact::Contact(float restitution, float penetration, Vector3D contactNormal, PhysicsObject* contactOne, PhysicsObject* contactTwo)
{
	m_Restitution = restitution;
	m_Penetration = penetration;
	m_ContactNormal = contactNormal;

	mp_ContactOne = contactOne;
	mp_ContactTwo = contactTwo;
}

Contact::~Contact()
{
}

void Contact::resolveVelocity(realNum deltaTime)
{
	realNum separatingVelocity = calculateSeparatingVelocity();

	if (separatingVelocity > 0)
	{
		return;
	}

	realNum newSeparatingVelocity = -separatingVelocity * m_Restitution;

	Vector3D acceleration = mp_ContactOne->getAcceleration();
	if (mp_ContactTwo)
	{
		acceleration -= mp_ContactTwo->getAcceleration();
	}

	realNum velocity = acceleration.dot(m_ContactNormal) * (realNum)deltaTime;
	if (velocity < 0)
	{
		separatingVelocity -= velocity;
	}

	realNum deltaVelocity = newSeparatingVelocity - separatingVelocity;

	realNum totalInverseMass = (realNum)mp_ContactOne->getInverseMass();
	if (mp_ContactTwo)
	{
		totalInverseMass += (realNum)mp_ContactTwo->getInverseMass();
	}

	if (totalInverseMass <= 0)
	{
		return;
	}

	realNum impulse = deltaVelocity / totalInverseMass;
	Vector3D impulsePerIMass = m_ContactNormal * impulse;

	mp_ContactOne->setVelocity(mp_ContactOne->getVelocity() + impulsePerIMass * (realNum)mp_ContactOne->getInverseMass());
	if (mp_ContactTwo)
	{
		mp_ContactTwo->setVelocity(mp_ContactTwo->getVelocity() + impulsePerIMass * (realNum)- mp_ContactTwo->getInverseMass());
	}
}

void Contact::resolveInterpenetration(realNum deltaTime)
{
	if (m_Penetration <= 0)
	{
		return;
	}

	realNum totalInverseMass = (realNum)mp_ContactOne->getInverseMass();
	if (mp_ContactTwo)
	{
		totalInverseMass += (realNum)mp_ContactTwo->getInverseMass();
	}

	if (totalInverseMass <= 0)
	{
		return;
	}

	Vector3D movementPerInverseMass = m_ContactNormal  * (m_Penetration / totalInverseMass);

	Vector3D contactOneMovement = movementPerInverseMass * (realNum)mp_ContactOne->getInverseMass() * deltaTime;
	mp_ContactOne->setPos(contactOneMovement + mp_ContactOne->getPos());
	if (mp_ContactTwo)
	{
		Vector3D contactTwoMovement = movementPerInverseMass * (realNum)-mp_ContactTwo->getInverseMass() * deltaTime;
		mp_ContactTwo->setPos(contactTwoMovement + mp_ContactTwo->getPos());
	}
}

float Contact::calculateSeparatingVelocity()
{
	Vector3D relativeVelocity = mp_ContactOne->getVelocity();
	if (mp_ContactTwo)
	{
		relativeVelocity -= mp_ContactTwo->getVelocity();
	}

	return relativeVelocity.dot(m_ContactNormal);
}

void Contact::Resolve(realNum deltaTime)
{
	resolveVelocity(deltaTime);
	resolveInterpenetration(deltaTime);
}