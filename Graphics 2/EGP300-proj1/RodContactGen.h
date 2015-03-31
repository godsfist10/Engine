#pragma once

#include "ContactHandler.h"

class RodContactGen : public ContactGenerator
{
public:
	RodContactGen();
	RodContactGen(PhysicsObject* object, PhysicsObject* otherObject, float length);
	~RodContactGen();

	virtual void AddContact(ContactHandler* collisionHandler);

protected:

	PhysicsObject* mObj;
	PhysicsObject* mObj2;
	float mLength;
	
};

