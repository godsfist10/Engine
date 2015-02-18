#pragma once

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>

#define GLM_FORCE_RADIANS
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

#include <Arc/Map.h>
#include "Shader.h"
#include "WaveShader.h"
#include "ColorBombShader.h"
#include "Diffuse.h"

using namespace std;
using namespace glm;
using namespace Arc;

class Shader_Manager
{
public:
	Shader_Manager(void);
	~Shader_Manager(void);
	void cleanup();

	void initAll();
	void addShader(const string& shaderName, Shader* shaderToAdd);
	void setupForDraw(const string& shaderName, mat4x4 mvpMatrix);
	void update(double deltaTime);

	inline bool hasShader(const string& key) { return mShaderMap.containsKey(key); }

private:

	Map<string, Shader*> mShaderMap;
};

