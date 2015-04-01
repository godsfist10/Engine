#include "ContactHandler.h"

ContactHandler::ContactHandler()
{
	m_MaxChecks = 4;
	m_Collisions = 0;
}

ContactHandler::~ContactHandler()
{
	cleanup();
}

void ContactHandler::cleanup()
{
	std::vector<ContactGenerator*>::iterator iter = mp_ContactGenerators.begin();
	for (; iter != mp_ContactGenerators.end();)
	{
		ContactGenerator* temp = *iter._Ptr;
		iter = mp_ContactGenerators.erase(iter);
		delete temp;
	}
}

int ContactHandler::checkCollisions()
{
	int contactCount = 0;
	std::vector<ContactGenerator*>::iterator contactGeneratorIter;
	for (contactGeneratorIter = mp_ContactGenerators.begin(); contactGeneratorIter != mp_ContactGenerators.end(); contactGeneratorIter++)
	{
		(*contactGeneratorIter)->AddContact(this);
		contactCount++;
	}

	return contactCount;
}

void ContactHandler::resolveContacts(realNum deltaTime)
{
	std::vector<Contact>::iterator contactIter;
	for (contactIter = m_Contacts.begin(); contactIter != m_Contacts.end(); contactIter++)
	{
		(*contactIter).Resolve(deltaTime);
	}

	m_Contacts.clear();
}

void ContactHandler::addRunTimeContactGenerators()
{
	std::vector<ContactGenerator*>::iterator contactGeneratorIter;
	for (contactGeneratorIter = mp_ToAddContactGenerators.begin(); contactGeneratorIter != mp_ToAddContactGenerators.end(); contactGeneratorIter++)
	{
		mp_ContactGenerators.push_back((*contactGeneratorIter));
	}

	mp_ToAddContactGenerators.clear();
}

void ContactHandler::update(double deltaTime)
{
	int checkCount = checkCollisions() * 2 - 1;
	m_Collisions = m_Contacts.size();
	resolveContacts((realNum)deltaTime);
	if (checkCount > m_MaxChecks)
	{
		checkCount = m_MaxChecks;  //no more checks please thank you
	}

	for (int index = 0; index < checkCount; index++)
	{
		checkCollisions();
		resolveContacts((realNum)deltaTime);
	}

	//add the generators that are on standby (if a particle entered the water and gets buoyancy added during runtime)
	addRunTimeContactGenerators();
}

void ContactHandler::clearContactGenerators()
{
	mp_ContactGenerators.clear();
	mp_ToAddContactGenerators.clear();
}

void ContactHandler::AddCollisionObject(PhysicsObject* physicsObject)
{
	mp_PhysicsObjects.push_back(physicsObject);
}

void ContactHandler::AddCollisionObjects(std::vector<PhysicsObject*> physicsObjects)
{
	std::vector<PhysicsObject*>::iterator physicsObjectIter;
	for (physicsObjectIter = physicsObjects.begin(); physicsObjectIter != physicsObjects.end(); physicsObjectIter++)
	{
		mp_PhysicsObjects.push_back(*physicsObjectIter);
	}
}

void ContactHandler::AddRunTimeContactGenerator(ContactGenerator* contactGeneraor)
{
	mp_ToAddContactGenerators.push_back(contactGeneraor);
}


void ContactHandler::AddContactGenerators(std::vector<ContactGenerator*> contactGenerators)
{
	mp_ContactGenerators.insert(mp_ContactGenerators.end(), contactGenerators.begin(), contactGenerators.end());
}
