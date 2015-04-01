#include "Contact.h"
#include "PhysicsObject.h"

Contact::Contact(float restitution, float penetration, Vector3D contactNormal, PhysicsObject* contactOne, PhysicsObject* contactTwo)
{
	mRestitution = restitution;
	mPenetration = penetration;
	mContactNormal = contactNormal;
	mpContactOne = contactOne;
	mpContactTwo = contactTwo;
}

Contact::~Contact()
{
}

void Contact::resolveVelocity(realNum deltaTime)
{
	realNum separatingVelocity = calculateSeparatingVelocity();

	//if objects are seperating theres not a problem
	if (separatingVelocity > 0)	return;
	
	realNum newSeparatingVelocity = -separatingVelocity * mRestitution;

	Vector3D acceleration = mpContactOne->getAcceleration();
	if (mpContactTwo)
	{
		acceleration -= mpContactTwo->getAcceleration();
	}

	realNum velocity = acceleration.dot(mContactNormal) * (realNum)deltaTime;
	if (velocity < 0)
	{
		separatingVelocity -= velocity;
	}

	realNum deltaVelocity = newSeparatingVelocity - separatingVelocity;

	realNum totalInverseMass = (realNum)mpContactOne->getInverseMass();
	if (mpContactTwo)
	{
		totalInverseMass += (realNum)mpContactTwo->getInverseMass();
	}

	if (totalInverseMass <= 0) return;
	
	realNum impulse = deltaVelocity / totalInverseMass;
	Vector3D impulseToAdd = mContactNormal * impulse;

	mpContactOne->setVelocity(mpContactOne->getVelocity() + impulseToAdd * (realNum)mpContactOne->getInverseMass());
	if (mpContactTwo)
	{
		mpContactTwo->setVelocity(mpContactTwo->getVelocity() + impulseToAdd * (realNum)-mpContactTwo->getInverseMass());
	}
}

void Contact::resolveInterpenetration(realNum deltaTime)
{
	//if its not coliding dont bother
	if (mPenetration <= 0) return;
	

	realNum totalInverseMass = (realNum)mpContactOne->getInverseMass();
	if (mpContactTwo)
	{
		totalInverseMass += (realNum)mpContactTwo->getInverseMass();
	}

	//if the inverse mass is less than zero you probably have a problem
	if (totalInverseMass <= 0)	return;

	Vector3D movementPerInverseMass = mContactNormal  * (mPenetration / totalInverseMass);

	Vector3D contactOneMovement = movementPerInverseMass * (realNum)mpContactOne->getInverseMass() * deltaTime;
	mpContactOne->setPos(contactOneMovement + mpContactOne->getPos());
	if (mpContactTwo)
	{
		Vector3D contactTwoMovement = movementPerInverseMass * (realNum)-mpContactTwo->getInverseMass() * deltaTime;
		mpContactTwo->setPos(contactTwoMovement + mpContactTwo->getPos());
	}
}

//nice function for calculating th seperation velocity for readability 
float Contact::calculateSeparatingVelocity()
{
	Vector3D relativeVelocity = mpContactOne->getVelocity();
	if (mpContactTwo)
	{
		relativeVelocity -= mpContactTwo->getVelocity();
	}

	return relativeVelocity.dot(mContactNormal);
}

void Contact::Resolve(realNum deltaTime)
{
	resolveVelocity(deltaTime);
	resolveInterpenetration(deltaTime);
}