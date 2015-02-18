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

void Debug::init(ResourceManager* resourceManager, const string& textMtlFile)
{
	resourceManager->loadMTLFile(textMtlFile);
	textMat = resourceManager->getMaterial("textMaterial");
}

void Debug::update()
{
	pruneExpiredMessages();
	double time = (double)glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	mDeltaTime = time - mTimePrev;
	mTimePrev = time;
	if( debugMode)
		addTextToScreen("delta time: " + std::to_string(getDeltaTime()), vec2( 10, screenHeight - 30), false, 0.0f);
	incrementFrame();
	
}

void Debug::incrementFrame()
{
	double currentTime = glutGet(GLUT_ELAPSED_TIME);
	mFpsCount++;
	if (currentTime - mLastTime >= 1000.0)
	{
		if ( debugMode)
			addTextToScreen(std::to_string(1000.0 / (double)mFpsCount) + " m/s per frame - FPS: " + std::to_string(mFpsCount), vec2(10,screenHeight - 50), false, 1.0f);
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


void Debug::addMessageToScreen(const string& message, bool saveLog)
{
	if (saveLog)
		log(message);

	onScreenMessage messageToAdd;
	messageToAdd.message = message;
	messageToAdd.timer = 1.0f;
	mScrollingMessages.push_back(messageToAdd);

}

void Debug::drawMessages()
{
	textMat->bindTexture();
	for (std::vector<onScreenMessage>::iterator it = mMessages.begin(); it != mMessages.end(); ++it)
	{
		onScreenMessage* temp = it._Ptr;
		
		glWindowPos2fARB(temp->pos.x, temp->pos.y);
		int len, j;
		len = (int)temp->message.size();
		for (j = 0; j < len; j++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, temp->message[j]);
		}
	}
	int i = 1;
	float textHeight = 22.0f;
	for (std::vector<onScreenMessage>::reverse_iterator it = mScrollingMessages.rbegin(); it != mScrollingMessages.rend(); ++it)
	{
	
		glWindowPos2fARB(10 , textHeight * i);
		int len, j;
		len = (int)it->message.size();
		for (j = 0; j < len; j++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, it->message[j]);
		}
		i++;
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

	std::vector<onScreenMessage>::iterator iter = mScrollingMessages.begin();

	for (; iter != mScrollingMessages.end();)
	{
		if (iter._Ptr->timer <= 0)
		{
			iter = mScrollingMessages.erase(iter);
		}
		else
		{
			iter->timer -= (float)mDeltaTime;
			++iter;
		}
	}
}
