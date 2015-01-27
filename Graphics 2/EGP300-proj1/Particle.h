#pragma once

#include "PhysicsObject.h"

using namespace std;
using namespace glm;
using namespace Arc;

class Particle : public PhysicsObject
{
public:
	Particle();
	Particle(float life);
	~Particle();
	
	virtual void update();
	virtual void onSpawn();
	virtual void onDeath();

protected:

	float lifeSpan;


};

