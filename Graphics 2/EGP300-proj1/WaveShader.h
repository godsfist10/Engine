#pragma once

#include "Shader.h"

class WaveShader : 
	public Shader
{
public:
	WaveShader();
	~WaveShader();

	void init();
	void setupForDraw(mat4x4 mvpMatrix);
	void update(double deltaTime);

protected:

	GLuint baseImageLoc;
	GLuint locColor;
	GLuint locTime;
	GLuint locWaveHeight;
	GLuint locWaveWidth;

	float waveTime;
	float waveWidth;
	float waveHeight;
};

