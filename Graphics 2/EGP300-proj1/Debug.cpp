#include "Debug.h"
#include <Windows.h>
#include <GL/glut.h>

Debug::Debug()
{
	mLastTime = glutGet(GLUT_ELAPSED_TIME);
	mFpsCount = 0;
	mDeltaTime = 0;
	mTimePrev = 0;
	debugMode = false;
}

Debug::~Debug()
{
	cleanup();
}

void Debug::cleanup()
{
	mMessages.clear();
}

void Debug::update()
{
	pruneExpiredMessages();
	double time = (double)glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	mDeltaTime = time - mTimePrev;
	mTimePrev = time;
	if( debugMode)
		addTextToScreen("delta time: " + std::to_string(getDeltaTime()), vec2( 10,30), false, 0.0f);
	incrementFrame();
	
}

void Debug::incrementFrame()
{
	double currentTime = glutGet(GLUT_ELAPSED_TIME);
	mFpsCount++;
	if (currentTime - mLastTime >= 1000.0)
	{
		if ( debugMode)
			addTextToScreen(std::to_string(1000.0 / (double)mFpsCount) + " m/s per frame - FPS: " + std::to_string(mFpsCount), vec2(10,10), false, 1.0f);
		mFpsCount = 0;
		mLastTime += 1000.0;
	}
	
}

void Debug::addTextToScreen(const string& message, vec2 screenPos, bool saveLog, float timer)
{
	if (saveLog)
		log(message);

	onScreenMessage messageToAdd;
	messageToAdd.message = message;
	messageToAdd.timer = timer;
	messageToAdd.pos = screenPos;

	mMessages.push_back(messageToAdd);
}

void Debug::drawMessages()
{
	for (std::vector<onScreenMessage>::iterator it = mMessages.begin(); it != mMessages.end(); ++it)
	{
		onScreenMessage* temp = it._Ptr;
		glColor3f(1.0f, 1.0f, 1.0f);
		glWindowPos2fARB(temp->pos.x, temp->pos.y);
		int len, j;
		len = (int)temp->message.size();
		for (j = 0; j < len; j++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, temp->message[j]);
		}
	}
}

void Debug::log(const string& log)
{
	cout << log << endl;
}

void Debug::pruneExpiredMessages()
{
	std::vector<onScreenMessage>::iterator it = mMessages.begin();

	for (; it != mMessages.end();) 
	{
		if (it._Ptr->timer <= 0) 
		{
			it = mMessages.erase(it);
		}
		else 
		{
			it->timer -= (float)mDeltaTime;
			++it;
		}
	}
}
