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

	glm::vec3 Velocity() const { return m_velocity; }
	void Velocity(glm::vec3 val) { m_velocity = val; }

	glm::vec3 Pos() const { return m_pos; }
	void Pos(glm::vec3 val) { m_pos = val; }

	float SpawnSpeed() const { return spawnSpeed; }
	void SpawnSpeed(float val) { spawnSpeed = val; }

protected:

	Map<string, BillboardedTexture*> particleMap;

	bool updated;
	float particleLifeSpan, randomizeMultiplier, spawnSpeed;
	
	
	vec3 m_velocity, m_pos;
	



};

