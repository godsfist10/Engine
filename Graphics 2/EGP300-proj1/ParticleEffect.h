#pragma once

#include "ResourceManager.h"

class ParticleEffect
{
public:
	ParticleEffect(void);
	~ParticleEffect(void);
	void loadEffect(const string& particleFilename, const string& particleSystemName, bool shouldUpdate);
	void burst(const int& burstAmount);
	void update();

	bool Updated() const { return updated; }
	void Updated(bool val) { updated = val; }

	float ParticleLifeSpan() const { return particleLifeSpan; }
	void ParticleLifeSpan(float val) { particleLifeSpan = val; }

	float RandomizeMultiplier() const { return randomizeMultiplier; }
	void RandomizeMultiplier(float val) { randomizeMultiplier = val; }

	float SpawnSpeed() const { return spawnSpeed; }
	void SpawnSpeed(float val) { spawnSpeed = val; }

protected:

	Map<string, PhysicsObject*> particleMap;

	bool updated;
	float particleLifeSpan, randomizeMultiplier, spawnSpeed;
	
	
	vec3 m_velocity, m_pos;
	



};

