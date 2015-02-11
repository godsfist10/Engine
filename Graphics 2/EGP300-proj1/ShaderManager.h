#pragma once

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>

#define GLM_FORCE_RADIANS
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>


using namespace std;
using namespace glm;

class Shader_Manager
{
public:
	Shader_Manager(void);
	~Shader_Manager(void);
	void init(const string& vertexProgramName, const string& fragmentProgramName);
	void setupForDraw(const string& shaderName, mat4x4 mvpMatrix);
	void update();

private:

	GLuint waterShader;
	bool waterShaderInUse;
	GLuint colorBombShader;
	bool colorBombShaderInUse;

	//wave vars
	GLuint baseImageLoc;
	GLuint locMVP; 
	GLuint locColor; 
	GLuint locTime; 
	GLuint locWaveHeight; 
	GLuint locWaveWidth; 

	float waveTime;
	float waveWidth;
	float waveHeight;
	float lastTime;

	//colorBomb Vars
	GLuint locColorBomb;
	GLuint locMVP_Bomb;

	float colorStep_R;
	float colorStep_G;
	float colorStep_B;
	float colorStepAmount;
	int colorIndex;
	
};

