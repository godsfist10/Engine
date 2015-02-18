#pragma once

#include "Shader.h"

class ColorBombShader :
	public Shader
{
public:
	ColorBombShader();
	~ColorBombShader();

	void init();
	void setupForDraw(mat4x4 mvpMatrix);
	void update(double deltaTime);

protected:

	GLuint locColorBomb;

	float colorStep_R;
	float colorStep_G;
	float colorStep_B;
	float colorStepAmount;
	int colorIndex;

};

