#include "Debug.h"
#include <Windows.h>
#include <GL/glut.h>

Debug::Debug()
{
	mLastFrameTime = glutGet(GLUT_ELAPSED_TIME);
	mStartTime = glutGet(GLUT_ELAPSED_TIME);
	log(std::to_string(mStartTime));
	mCurrentFrameCount = 0;
}

Debug::~Debug()
{
	cleanup();
}

void Debug::cleanup()
{
	mMessages.clear();
}

bool Debug::fpsCap()
{
	double endFrameTime = mStartTime + (mCurrentFrameCount + 1) * TIME_PER_FRAME;
	double endRenderTime = glutGet(GLUT_ELAPSED_TIME);
	double idleTime = endFrameTime - endRenderTime;
	if (idleTime <= 0.0)
	{
		return true;
	}
	return false;
}

void Debug::incrementFrame()
{
	mCurrentFrameCount++;
	mLastFrameTime = glutGet(GLUT_ELAPSED_TIME);
	pruneExpiredMessages();
}

float Debug::getDeltaTime()
{
	int renderTime = glutGet(GLUT_ELAPSED_TIME);
	return ((float)renderTime - mLastFrameTime) / 100.0f;
}

float Debug::getFPS()
{
	return getDeltaTime();
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
		glRasterPos2f(temp->pos.x, temp->pos.y);
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
			++it;
		}
	}
}
