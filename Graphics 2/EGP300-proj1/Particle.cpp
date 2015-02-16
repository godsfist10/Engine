#include "Particle.h"


Particle::Particle()
{
	killMe = false;
}

Particle::Particle(const Map<string, Model*> &modelsMap, const float& life)
	:PhysicsObject(modelsMap)
{
	lifeSpan = life;
	killMe = false;
}

Particle::~Particle()
{

}

void Particle::update(double deltaTime)
{
	lifeSpan--;
	if(lifeSpan <= 0)
		killMe = true;

	physicsUpdate(deltaTime);
}
