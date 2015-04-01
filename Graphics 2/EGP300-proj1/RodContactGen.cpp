#include "RodContactGen.h"


RodContactGen::RodContactGen()
{
}

RodContactGen::RodContactGen(PhysicsObject* object, PhysicsObject* otherObject, float length)
{
	mObj = object;
	mObj2 = otherObject;
	mLength = length;
}

RodContactGen::~RodContactGen()
{
}

void RodContactGen::AddContact(ContactHandler* contactHandler)
{
	//find current dist
	float currentLength = Vector3D::dist(mObj->getPos(), mObj2->getPos());
	
	//check if objects are already perfect distance apart + return if so
	if (currentLength == mLength) return;

	//rest = 0 so that there is no mercy
	realNum restitution = 0.0f;
	realNum penetration = 0.0f;
	Vector3D contactNormal = mObj->getPos() - mObj2->getPos();
	contactNormal.normalize();

	//if farther apart
	if (currentLength > mLength)
	{
		penetration = currentLength - mLength;
		contactNormal *= -1.0f;
	}
	else //if closer together
	{
		penetration = mLength - currentLength;
	}

	//add contact
	contactHandler->AddContact(Contact(restitution, penetration, contactNormal, mObj, mObj2));

}