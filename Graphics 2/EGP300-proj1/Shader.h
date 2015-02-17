#pragma once

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>

#define GLM_FORCE_RADIANS
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

#include <string>

using namespace std;
using namespace glm;

class Shader
{
public:
	Shader();
	~Shader();
	virtual void init(string vertShader, string fragShader);
	virtual void setupForDraw(mat4x4 mvpMatrix);
	virtual void update(double deltaTime);
protected:

	GLuint mShaderIndex;

	GLuint locMVP;
};

