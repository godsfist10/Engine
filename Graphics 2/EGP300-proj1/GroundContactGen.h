#pragma once

#include "ContactGenerator.h"
#include "ContactHandler.h"

class GroundContactGen :public ContactGenerator
	{
public:
	GroundContactGen();
	GroundContactGen(float groundY);
	virtual ~GroundContactGen();

	void AddContact(ContactHandler* contactHandler);
	
	inline void setHeight(float yPos) { mHeight = yPos; };
	inline float getHeight() { return mHeight; };

protected:

	float mHeight;

};

