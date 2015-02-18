#include "WaveShader.h"


WaveShader::WaveShader()
{
}


WaveShader::~WaveShader()
{
}

void WaveShader::init()
{
	waveTime = 0.0f;
	waveWidth = 0.03f;
	waveHeight = 3.0f;

	mShaderIndex = gltLoadShaderPairWithAttributes("WaterVertShader.vp", "WaterFragShader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "texCoord");

	locColor = glGetUniformLocation(mShaderIndex, "vColorValue");
	locMVP = glGetUniformLocation(mShaderIndex, "mvpMatrix");
	locTime = glGetUniformLocation(mShaderIndex, "waveTime");
	locWaveWidth = glGetUniformLocation(mShaderIndex, "waveWidth");
	locWaveHeight = glGetUniformLocation(mShaderIndex, "waveHeight");
	baseImageLoc = glGetUniformLocation(mShaderIndex, "baseImage");
}

void WaveShader::setupForDraw(mat4x4 mvpMatrix)
{
	float colorWater[] = { 0.145f, 0.427f, 0.482f, 0.8f };

	glUseProgram(mShaderIndex);
	glUniform1i(baseImageLoc, 0);
	glUniform1f(locTime, waveTime);
	glUniform1f(locWaveWidth, waveWidth);
	glUniform1f(locWaveHeight, waveHeight);
	glUniform4fv(locColor, 1, colorWater);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
}

void WaveShader::update(double deltaTime)
{
	waveTime += (float)deltaTime;
}
