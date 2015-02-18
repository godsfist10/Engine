#include "Diffuse.h"


Diffuse::Diffuse()
{
}


Diffuse::~Diffuse()
{
}

void Diffuse::init()
{
	mShaderIndex = gltLoadShaderPairWithAttributes("DiffuseVertShader.vp", "DiffuseFragShader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "texCoord");
	locMVP = glGetUniformLocation(mShaderIndex, "mvpMatrix");
	baseImageLoc = glGetUniformLocation(mShaderIndex, "baseImage");
}

void Diffuse::setupForDraw(mat4x4 mvpMatrix)
{
	glUseProgram(mShaderIndex);
	glUniform1i(baseImageLoc, 0);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
}