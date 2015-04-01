#include "GroundContactGen.h"


GroundContactGen::GroundContactGen()
{
	mHeight = 0;
}

GroundContactGen::GroundContactGen(float groundHeight)
{
	mHeight = groundHeight;
}

GroundContactGen::~GroundContactGen()
{
}

void GroundContactGen::AddContact(ContactHandler* contactHandler)
{
	//goes through all collidable objects and tests if they are below the ground height axis.
	std::vector<PhysicsObject*> physicsObjects = contactHandler->GetPhysicsObjects();
	std::vector<PhysicsObject*>::iterator physicsObjectIter;
	for (physicsObjectIter = physicsObjects.begin(); physicsObjectIter != physicsObjects.end(); physicsObjectIter++)
	{
		float physicsObjectY = (*physicsObjectIter)->getPos().y;

		if (physicsObjectY < mHeight)
		{
			// if they are below height then apply collision with an upward contact normal with physics object restitution value
			float penetration = mHeight - physicsObjectY;
			contactHandler->AddContact(Contact((*physicsObjectIter)->getRestitution(), penetration, Vector3D(0, 1, 0), (*physicsObjectIter)));
		}
	}
}