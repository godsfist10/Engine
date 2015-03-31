#pragma once

class ContactHandler;

//base class for contact generators. has pure virtual addcontact class to be expanded upon

class ContactGenerator
{
public:
	ContactGenerator();
	virtual ~ContactGenerator();
	//why return a number when you can just send in the pointer like so. much more direct / neat imo
	virtual void AddContact(ContactHandler* collisionHandler) = 0;
};

