#include "ForceGenerator.h"


ForceGenerator::ForceGenerator()
{
}


ForceGenerator::~ForceGenerator()
{
}

void ForceGenerator::removeFromRegistry(PhysicsObject* object)
{
	std::vector<PhysicsObject*>::iterator it = mRegistry.begin(); //not sure if this works

	for (; it != mRegistry.end();)
	{
		if (*it._Ptr == object)
		{
			it = mRegistry.erase(it);
		}
		else
		{
			++it;
		}
	}
}