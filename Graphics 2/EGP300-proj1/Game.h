#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#undef main

#include <Arc/ArcCore.h>

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "Camera.h"
#include "Debug.h"
#include "Model.h"
#include "ResourceManager.h"
#include "Skybox.h"
#include "Heightmap.h"
#include "TerrainManager.h"
#include "BillboardedTexture.h"
#include "ParticleEffect.h"
#include "ShaderManager.h"
#include "ParticleForceRegistry.h"
#include "ContactHandler.h"

#include <vector>
#include <map>
#include <math.h>

#include <Arc/Map.h>

const float PI = 3.1415926f;
const float MAXFLOAT = std::numeric_limits<float>::max();
using namespace Arc;

class Game
{

public:
	Game(void);
	~Game(void);

	void start(int argNum, char* args[]);
	void endGame();
	void setUpWorld(int argNum, char* args[]);
	void update();
	void render();
	
	void ResetCamera();

	void hookResize( int width, int height );
	void hookKey(unsigned char key, int x, int y);
	void hookSpecialKey(int key, int x, int y);
	void hookMouseMoved( int x, int y );	
	void hookMousePassive( int x, int y);


#pragma region getters and setters

	inline Camera* getCamera() { return mpCamera; }

	inline int getScreenHeight() const { return height; }
	inline void setScreenHeight(int screenHeight) { height = screenHeight; }
	
	inline int getScreenWidth() const { return width; }
	inline void setScreenWidth(int screenWidth) { width = screenWidth; }

#pragma endregion
	
protected:

#pragma region memberVariables

	Camera* mpCamera;
	Debug* mpDebug;
	GLShaderManager	shaderManager;
	Shader_Manager* mpShaderManager;
	ResourceManager* mpResourceManager;
	TerrainManager* mpTerrainManager;
	ParticleForceRegistry* mpParticleForceManager;
	ContactHandler* mpContactHandler;

	mat4x4	mvpMatrix;
	GLint			width, height;

	GLfloat m_MouseX, m_MouseY;
	GLfloat m_CameraMoveSpeed, m_CameraLookSpeed;
	int systemTimeAdjuster;


	Skybox* m_underWaterSkybox;
	Skybox* m_cloudSkybox;

	Heightmap* m_heightmap;
	Heightmap* m_heightmap2;
	Heightmap* m_waterMap;

	PhysicsObject* physhy;
	ParticleEffect* PhyshyFriends;
	PhysicsObject* Earth;
	PhysicsObject* Earth2;
	PhysicsObject* Earth3;
	PhysicsObject* Earth4;
	PhysicsObject* Earth5;
	PhysicsObject* Earth6;

	
	PhysicsObject* Ground;


	bool mouseFree;
	bool Paused;
	bool help;
	bool fog;
	bool wireframe;
	bool fullscreen;
	vec4 fullScreen_Saves;
#pragma endregion

private:

	void PausedUpdate(double deltaTime);
	void UnpausedUpdate(double deltaTime);
	void FixedUpdate(double deltaTime);
	void WorldUpdate(double deltaTime);
	void WorldFixedUpdate(double deltaTime);
	void WorldDebug();
	void ResetWorld();
};

