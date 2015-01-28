#include "ParticleEffect.h"


ParticleEffect::ParticleEffect(void)
{
	totalParticleCount = 0;
}

ParticleEffect::ParticleEffect(ResourceManager* resourceManager, const string& name, vec3 pos, const float& lifespan, const float& spawnFrequency , vec3 startVel /*= vec3(0,0,0)*/, vec3 startAcc /*= vec3(0,0,0)*/)
{
	totalParticleCount = 0;
	spawnTimer = spawnSpeed;
	mpResourceManager = resourceManager;

	EffectName = name;
	particleLifeSpan = lifespan;
	spawnSpeed = spawnFrequency;
	particleStartvelocity = startVel;
	particleStartAcceleration = startAcc;
}


ParticleEffect::~ParticleEffect(void)
{
}

void ParticleEffect::cleanup()
{
	prefabModel.clear();
	particleMap.clear();
}

//For loading a new model
void ParticleEffect::loadEffect(const string& particleFilename, const string& particleEffectName)
{
	mpResourceManager->LoadFile(particleFilename, particleEffectName);
	prefabModel = mpResourceManager->getObject(particleEffectName)->getModelMap();
}

//for starting the effect with an already added model
void ParticleEffect::startEffect(const string& modelPrefabKey)
{
	prefabModel = mpResourceManager->getObject(modelPrefabKey)->getModelMap();
}

//create a burst of particles - after randomization has been created
void ParticleEffect::burst(const int& burstAmount)
{

}

//checks for particle spawning and dead particles
void ParticleEffect::update()
{
	spawnTimer--; 
	if( spawnTimer <= 0)
	{
		spawnTimer = spawnSpeed;
		createParticle();
	}

	Map<string, Particle*> deleteMap;

	for( auto it = particleMap.itBegin(); it != particleMap.itEnd(); ++it)
	{
		Particle* particle = it->second;
		if( particle->killMe)
		{
			onDeathBehavior(particle);
			deleteMap.add(it->first, particle);
		}
	}

	if (deleteMap.getSize() > 0)
	{
		for (auto it = deleteMap.itBegin(); it != deleteMap.itEnd(); ++it)
		{
			mpResourceManager->deleteObject(it->first);
			particleMap.removeKey(it->first);
		}
		deleteMap.clear();
	}

}

Particle* ParticleEffect::createParticle()
{
	string name = EffectName + std::to_string(totalParticleCount);
	totalParticleCount++;
	return createParticle(name);
}

Particle* ParticleEffect::createParticle(const string& key)
{
	Particle* tempParticle = mpResourceManager->addNewParticle(key, prefabModel, particleLifeSpan);
	particleMap.add(key, tempParticle);
	tempParticle->setAcceleration(particleStartAcceleration);
	tempParticle->setVelocity(particleStartvelocity);
	tempParticle->setPos(m_pos);
	return tempParticle;
}

void ParticleEffect::onDeathBehavior(Particle* dyingParticle)
{
	if( dyingParticle->getScale().x > .5f)
	{
		Particle* temp = createParticle();
		temp->setVelocity(vec3(0,1,0));
		temp->setScale(vec3(.5,.5,.5));
		temp->setPos(dyingParticle->getPos());
		temp = createParticle();
		temp->setVelocity(vec3(0,-1,0));
		temp->setScale(vec3(.5,.5,.5));
		temp->setPos(dyingParticle->getPos());
		temp = createParticle();
		temp->setVelocity(vec3(0,0,1));
		temp->setScale(vec3(.5,.5,.5));
		temp->setPos(dyingParticle->getPos());

	}
	
	
}



