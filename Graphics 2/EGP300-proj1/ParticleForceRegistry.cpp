#include "ParticleForceRegistry.h"


ParticleForceRegistry::ParticleForceRegistry()
{
}


ParticleForceRegistry::~ParticleForceRegistry()
{
	clearAllRegistrations();
	cleanupAllForceGenerators();
}

void ParticleForceRegistry::add(PhysicsObject* particle, ParticleForceGenerator *forceGenerator)
{
	ParticleForceRegistration temp;
	temp.forceGenerator = forceGenerator;
	temp.particle = particle;
	mRegistrations.push_back(temp);
}

void ParticleForceRegistry::updateForces(double deltaTime)
{
	Registry::iterator it = mRegistrations.begin();
	for (; it != mRegistrations.end(); it++)
	{
		it->forceGenerator->updateForce(it->particle, deltaTime);
	}
}

void ParticleForceRegistry::remove(PhysicsObject* particle, ParticleForceGenerator *forceGenerator)
{
	Registry::iterator it = mRegistrations.begin();

	for (; it != mRegistrations.end();)
	{
		if (it._Ptr->forceGenerator == forceGenerator)
		{
			if (it._Ptr->particle == particle)
			{
				it = mRegistrations.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}

void ParticleForceRegistry::cleanupAllForceGenerators()
{
	PFG::iterator it = mPFGList.begin();

	for (; it != mPFGList.end();)
	{
		ParticleForceGenerator* temp = *it._Ptr;
		it = mPFGList.erase(it);
		delete temp;
	}
}

void ParticleForceRegistry::addForceGeneratorToList(ParticleForceGenerator* toAdd)
{
	mPFGList.push_back(toAdd);
}
