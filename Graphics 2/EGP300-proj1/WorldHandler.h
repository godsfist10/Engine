#ifndef WORLDHANDLER_H
#define WORLDHANDLER_H

#include "ContactHandler.h"
#include "ResourceManager.h"
#include "ParticleForceRegistry.h"
#include "TerrainManager.h"
#include "Debug.h"

const float MAX_FLOAT = std::numeric_limits<float>::max();

class WorldHandler
{
public:
	WorldHandler();
	virtual ~WorldHandler();
	
	inline void setGravGen(GravityForceGenerator* gravGen) { mGravGen = gravGen; }
	void loadTextFile(string filename, ResourceManager* resourceManager, ContactHandler* contactHandler, ParticleForceRegistry* particleForceManager);

protected:

	GravityForceGenerator* mGravGen;
};

#endif //WORLDHANDLER_H