#pragma once
#include "ResourceManager.h"

using namespace std;


struct onScreenMessage
{
	float timer;
	string message;
	vec2 pos;
};


class Debug
{
public:
	Debug();
	~Debug();
	void cleanup();
	void init();

	const float TIME_PER_FRAME = 1000.0f / 60.0f;
	static void log(const string& log);
	
	bool fpsCap();
	void incrementFrame();
	
	void addTextToScreen(const string& message, vec2 screenPos, bool saveLog = false, float timer = 0);
	void drawMessages();
	
	float getDeltaTime();
	float getFPS();
	inline int getCurrentFrame() { return mCurrentFrameCount; } 
	

protected:

	vector<onScreenMessage> mMessages;

	void pruneExpiredMessages();

	int mLastFrameTime;
	int mStartTime;
	int mCurrentFrameCount;


};

