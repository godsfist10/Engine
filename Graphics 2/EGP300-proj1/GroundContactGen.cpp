#include "GroundContactGen.h"


GroundContactGen::GroundContactGen()
{
	mHeight = 0;
}

GroundContactGen::GroundContactGen(float groundHeight)
{
	mHeight = groundHeight;
}

//-----------------------------------------------------------------------------
GroundContactGen::~GroundContactGen()
{
}

//-----------------------------------------------------------------------------
void GroundContactGen::AddContact(ContactHandler* contactHandler)
{
	std::vector<PhysicsObject*> physicsObjects = contactHandler->GetPhysicsObjects();
	std::vector<PhysicsObject*>::iterator physicsObjectIter;
	for (physicsObjectIter = physicsObjects.begin(); physicsObjectIter != physicsObjects.end(); physicsObjectIter++)
	{
		float physicsObjectY = (*physicsObjectIter)->getPos().y;

		if (physicsObjectY < mHeight)
		{
			float penetration = mHeight - physicsObjectY;
			contactHandler->AddContact(Contact((*physicsObjectIter)->getRestitution(), penetration, Vector3D(0, 1, 0), (*physicsObjectIter)));
		}
	}
}