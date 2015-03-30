#include <vector>
#include "ParticleForceGenerator.h"

//including all types of force gen here so as not to clutter the game.h include space
#include "AnchorSpringForceGen.h"
#include "DragForceGenerator.h"
#include "ElasticForceGen.h"
#include "GravityForceGenerator.h"
#include "SpringForceGen.h"
#include "BuoyancyForceGen.h"


//Holds all the force generators and the particles they apply to - from the book
class ParticleForceRegistry
{
protected:

	struct ParticleForceRegistration
	{
		PhysicsObject* particle;
		ParticleForceGenerator* forceGenerator;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry mRegistrations;

	typedef std::vector<ParticleForceGenerator*> PFG;
	PFG mPFGList;

public:
	ParticleForceRegistry();
	~ParticleForceRegistry();

	/**
	* Registers the given force generator to apply to the
	* given particle.
	*/
	void add(PhysicsObject* particle, ParticleForceGenerator *forceGenerator);
	
	/**
	* Removes the given registered pair from the registry.
	* If the pair is not registered, this method will have
	* no effect.
	*/
	void remove(PhysicsObject* particle, ParticleForceGenerator *forceGenerator);
	
	/**
	* Clears all registrations from the registry. This will
	* not delete the particles or the force generators
	* themselves, just the records of their connection.
	*/
	inline void clearAllRegistrations() { mRegistrations.clear(); }
	
	/*
	Deletes all force generators in the list for endgame cleanup purposes
	*/
	void cleanupAllForceGenerators();

	void addForceGeneratorToList(ParticleForceGenerator* toAdd);

	/**
	* Calls all the force generators to update the forces of
	* their corresponding particles.
	*/
	void updateForces(double deltaTime);

};

