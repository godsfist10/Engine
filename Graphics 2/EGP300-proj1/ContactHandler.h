#pragma once

#include "Contact.h"
#include "ContactGenerator.h"
#include "GroundContactGen.h"
#include "RodContactGen.h"

class ContactGenerator;

class ContactHandler
{
private:
	std::vector<PhysicsObject*> mp_PhysicsObjects;
	std::vector<ContactGenerator*> mp_ContactGenerators;
	std::vector<ContactGenerator*> mp_ToAddContactGenerators;
	std::vector<Contact> m_Contacts;

	int m_MaxChecks;
	int m_Collisions;

private:
	int checkCollisions();
	void resolveContacts(float deltaTime);
	void addRunTimeContactGenerators();

public:
	ContactHandler();
	~ContactHandler();

	void cleanup();
	void Update(double deltaTime);
	void Reset();

	void AddGround(PhysicsObject* groundObject);
	void AddCollisionObject(PhysicsObject* physicsObject);
	void AddCollisionObjects(std::vector<PhysicsObject*> physicsObjects);

	void AddRunTimeContactGenerator(ContactGenerator* contactGeneraor);
	void AddContactGenerators(std::vector<ContactGenerator*> contactGenerators);

	std::vector<PhysicsObject*> GetPhysicsObjects() { return mp_PhysicsObjects; };

	void AddContact(Contact contact) { m_Contacts.push_back(contact); };

	inline int GetCollisions() { return m_Collisions; };
};
