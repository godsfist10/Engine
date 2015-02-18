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
	virtual void init() = 0;
	virtual void init(const string& vertShaderName, const string& fragShaderName, bool vertexAtt = false, bool textureAtt = false);
	virtual void setupForDraw(mat4x4 mvpMatrix) = 0;
	virtual void update(double deltaTime);
protected:

	GLuint mShaderIndex;

	GLuint locMVP;
};

