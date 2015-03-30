#include "BuoyancyForceGen.h"
#include "Debug.h"

BuoyancyForceGen::BuoyancyForceGen()
{
}

BuoyancyForceGen::BuoyancyForceGen(realNum maxDepth, realNum volume, realNum waterHeight, realNum liquidDensity)
{
	mMaxDepth = maxDepth;
	mVolume = volume;
	mWaterHeight = waterHeight;
	mLiquidDensity = liquidDensity;
}

BuoyancyForceGen::~BuoyancyForceGen()
{
}

void BuoyancyForceGen::updateForce(PhysicsObject* particle, double deltaTime)
{
	// Calculate the submersion depth.
	realNum depth = particle->getPos().y;
	// Check if we’re out of the water.
	if (depth >= mWaterHeight + mMaxDepth) return;
	Vector3D force(0, 0, 0);
	// Check if we’re at maximum depth.
	if (depth <= mWaterHeight - mMaxDepth)
	{
		
		force.Y = mLiquidDensity * mVolume;
		Debug::log(std::to_string(force.Y));
		particle->addForce(force);
		return;
	}
	// Otherwise we are partly submerged.
	force.Y = mLiquidDensity * mVolume * (depth - mMaxDepth - mWaterHeight) / (-2.0f * mMaxDepth);
	Debug::log(std::to_string(force.Y));
	particle->addForce(force);

}