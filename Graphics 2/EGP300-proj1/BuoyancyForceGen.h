#pragma once
#include "ParticleForceGenerator.h"
class BuoyancyForceGen :
	public ParticleForceGenerator
{
protected:
	/**
	* The maximum submersion depth of the object before
	* it generates its maximum buoyancy force.
	*/
	realNum mMaxDepth;
	/**
	* The volume of the object.
	*/
	realNum mVolume;
	/**
	* The height of the water plane above y=0. The plane will be
	* parallel to the XZ plane.
	*/
	realNum mWaterHeight;
	/**
	* The density of the liquid. Pure water has a density of
	* 1000 kg per cubic meter.
	*/
	realNum mLiquidDensity;

public:
	BuoyancyForceGen();
	
	BuoyancyForceGen(realNum maxDepth, realNum volume, realNum waterHeight, realNum liquidDensity = 1000.0f);

	virtual ~BuoyancyForceGen();

	virtual void updateForce(PhysicsObject* particle, double deltaTime);
};

