#include "Diffuse.h"


Diffuse::Diffuse()
{
}


Diffuse::~Diffuse()
{
}

void Diffuse::init()
{
	Shader::init("DiffuseVertShader.vp", "DiffuseFragShader.fp", true, true);

	baseImageLoc = glGetUniformLocation(mShaderIndex, "baseImage");
}

void Diffuse::setupForDraw(mat4x4 mvpMatrix)
{
	glUseProgram(mShaderIndex);
	glUniform1i(baseImageLoc, 0);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
}