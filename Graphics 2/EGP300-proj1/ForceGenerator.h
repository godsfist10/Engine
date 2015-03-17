#pragma once
#include "PhysicsObject.h"

class ForceGenerator
{
public:
	ForceGenerator();
	virtual ~ForceGenerator();

	virtual void update() = 0;

	inline vector<PhysicsObject*> getRegistry() { return mRegistry; }
	inline void addToRegistry(PhysicsObject* object) { mRegistry.push_back(object); }
	//inline void addToRegistry(Map<string, PhysicsObject*> objectMap) { mRegistry.insert(objectMap.itBegin, objectMap.itEnd); }
	void removeFromRegistry(PhysicsObject* object);

	inline void setIsGlobal(bool val) { mIsGlobal = val; }
	inline bool getIsGlobal() { return mIsGlobal; }

protected:

	virtual void addForce(PhysicsObject* obj) = 0;
	bool mIsGlobal;
	vector<PhysicsObject*> mRegistry;

};

