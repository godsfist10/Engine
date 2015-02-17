#include "Shader.h"


Shader::Shader()
{
}


Shader::~Shader()
{
}

void Shader::init(string vertShader, string fragShader)
{

	mShaderIndex = gltLoadShaderPairWithAttributes(vertShader.c_str(), fragShader.c_str());
	
	locMVP = glGetUniformLocation(mShaderIndex, "mvpMatrix");
}

void Shader::setupForDraw(mat4x4 mvpMatrix)
{
	glUseProgram(mShaderIndex);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
}

void Shader::update(double deltaTime)
{

}