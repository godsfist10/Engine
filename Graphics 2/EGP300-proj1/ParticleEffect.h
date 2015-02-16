#pragma once

#include "ResourceManager.h"

using namespace std;

class ParticleEffect
{
public:
	ParticleEffect(void);
	ParticleEffect(ResourceManager* resourceManager, const string& name, vec3 pos, const float& lifespan, const float& spawnFrequency , vec3 startVel = vec3(0,0,0), vec3 startAcc = vec3(0,0,0));
	~ParticleEffect(void);
	void cleanup();
	void loadEffect(const string& particleFilename, const string& particleEffectName);
	void startEffect(const string& modelPrefabKey);
	void burst(const int& burstAmount);
	
	virtual void update(double deltaTime);
	virtual void onDeathBehavior(Particle* dyingParticle);

	float ParticleLifeSpan() const { return particleLifeSpan; }
	void ParticleLifeSpan(float val) { particleLifeSpan = val; }

	float RandomizeMultiplier() const { return randomizeMultiplier; }
	void RandomizeMultiplier(float val) { randomizeMultiplier = val; }

	float SpawnSpeed() const { return spawnSpeed; }
	void SpawnSpeed(float val) { spawnSpeed = val; }

protected:

	Particle* createParticle();
	Particle* createParticle(const string& key);

	Map<string, Particle*> particleMap;
	Map<string, Model*> prefabModel;
	ResourceManager* mpResourceManager;

	//Map<string, BillboardedPhysicsObject*> billboardedParticleMap;
	string EffectName;
	float particleLifeSpan, randomizeMultiplier, spawnSpeed, spawnTimer;
	int totalParticleCount;
	vec3 particleStartvelocity, particleStartAcceleration, m_pos;

};

