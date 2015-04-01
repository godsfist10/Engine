#pragma once

#include "Contact.h"
#include "ContactGenerator.h"
#include "GroundContactGen.h"
#include "RodContactGen.h"

class ContactGenerator;

class ContactHandler
{
public:
	ContactHandler();
	~ContactHandler();

	void cleanup();
	void update(double deltaTime);
	void clearContactGenerators();

	void AddCollisionObject(PhysicsObject* physicsObject);
	void AddCollisionObjects(std::vector<PhysicsObject*> physicsObjects);

	void AddRunTimeContactGenerator(ContactGenerator* contactGeneraor);
	void AddContactGenerators(std::vector<ContactGenerator*> contactGenerators);

	inline std::vector<PhysicsObject*> GetPhysicsObjects() { return mp_PhysicsObjects; };
	inline void AddContact(Contact contact) { m_Contacts.push_back(contact); };
	inline int GetCollisions() { return m_Collisions; };

private:
	std::vector<PhysicsObject*> mp_PhysicsObjects;
	std::vector<ContactGenerator*> mp_ContactGenerators;
	std::vector<ContactGenerator*> mp_ToAddContactGenerators;
	std::vector<Contact> m_Contacts;

	int m_MaxChecks;
	int m_Collisions;

	//other classes dont need these they are all internal
	int checkCollisions();
	void resolveContacts(float deltaTime);
	void addRunTimeContactGenerators();
};
