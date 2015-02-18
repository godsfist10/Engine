#pragma once
#include "ResourceManager.h"

using namespace std;

class ResourceManager;

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
	void init(ResourceManager* resourceManager, const string& textMtlFile);

	const float TIME_PER_FRAME = 1000.0f / 60.0f;
	static void log(const string& log);
	
	void update();
	void incrementFrame();
	
	void addTextToScreen(const string& message, vec2 screenPos, bool saveLog = false, float timer = 0);
	void addMessageToScreen(const string& message, bool saveLog = false);
	void drawMessages();
	
	inline double getDeltaTime() { return mDeltaTime; }
	inline void toggleDebugMode() { debugMode = !debugMode; }
	inline bool getDebugMode() { return debugMode; }
	//inline int getCurrentFrame() { return mCurrentFrameCount; } 

	int screenWidth;
	int screenHeight;

protected:

	vector<onScreenMessage> mMessages;
	vector<onScreenMessage> mScrollingMessages;

	void pruneExpiredMessages();
	bool debugMode;

	double mLastTime;
	double mDeltaTime;
	double mTimePrev;
	int mFpsCount;

	Material* textMat;
};

