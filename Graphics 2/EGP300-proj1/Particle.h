#pragma once

#include "PhysicsObject.h"

using namespace std;
using namespace glm;
using namespace Arc;

class Particle : public PhysicsObject
{
public:
	Particle();
	Particle(const Map<string, Model*> &modelsMap, const float& life);
	~Particle();
	

	virtual void update(double deltaTime);

	bool killMe;

protected:

	float lifeSpan;


};

