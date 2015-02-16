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

class Shader
{
public:
	Shader();
	~Shader();
	virtual void init(string vertShader, string fragShader);

protected:

	GLuint mShaderIndex;

	GLuint locMVP;
};

