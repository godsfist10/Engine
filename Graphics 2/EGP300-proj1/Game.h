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
#include "Model.h"
#include "ResourceManager.h"
#include "Skybox.h"
#include "Heightmap.h"
#include "TerrainManager.h"
#include "BillboardedTexture.h"
#include "ShaderManager.h"
#include <vector>
#include <map>
#include <math.h>

#include <Arc/Map.h>

const float PI = 3.1415926f;

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
	void PausedUpdate();
	void UnpausedUpdate();
	void FixedUpdate();
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
	GLShaderManager	shaderManager;
	Shader_Manager* waterShaderManager;
	ResourceManager* mpResourceManager;
	TerrainManager* mpTerrainManager;

	mat4x4	mvpMatrix;
	GLint			width, height;
	

	GLfloat m_MouseX, m_MouseY;
	GLfloat m_CameraMoveSpeed, m_CameraLookSpeed;

	Skybox* m_underWaterSkybox;
	Skybox* m_cloudSkybox;

	Heightmap* m_heightmap;
	Heightmap* m_heightmap2;
	Heightmap* m_waterMap;

	bool mouseFree;
	bool Paused;
	bool fog;
	bool wireframe;
	bool fullscreen;
	vec4 fullScreen_Saves;
#pragma endregion

};

