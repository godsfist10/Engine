#include "ColorBombShader.h"


ColorBombShader::ColorBombShader()
{
}


ColorBombShader::~ColorBombShader()
{
}

void ColorBombShader::init()
{
	colorStepAmount = .05f;
	colorIndex = 0;

	colorStep_R = 1;
	colorStep_G = 0;
	colorStep_B = 0;

	mShaderIndex = gltLoadShaderPairWithAttributes("ColorVertShader.vp", "ColorFragShader.fp", 1, GLT_ATTRIBUTE_VERTEX, "vVertex");

	locColorBomb = glGetUniformLocation(mShaderIndex, "vColorBombValue");
	locMVP = glGetUniformLocation(mShaderIndex, "mvpMatrix");

}

void ColorBombShader::setupForDraw(mat4x4 mvpMatrix)
{
	float colorBomb[] = { colorStep_R, colorStep_G, colorStep_B, 1.0f };

	glUseProgram(mShaderIndex);
	glUniform4fv(locColorBomb, 1, colorBomb);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
}

void ColorBombShader::update(double deltaTime)
{
	if (colorIndex == 0)
	{
		colorStep_G += colorStepAmount;
		if (colorStep_G >= 1)
		{
			colorIndex++;
			colorStep_G = 1.0f;
		}

	}
	else if (colorIndex == 1)
	{
		colorStep_R -= colorStepAmount;
		if (colorStep_R <= 0)
		{
			colorIndex++;
			colorStep_R = 0.0f;
		}
	}
	else if (colorIndex == 2)
	{
		colorStep_B += colorStepAmount;
		if (colorStep_B >= 1)
		{
			colorIndex++;
			colorStep_B = 1.0f;
		}
	}
	else if (colorIndex == 3)
	{
		colorStep_G -= colorStepAmount;
		if (colorStep_G <= 0)
		{
			colorIndex++;
			colorStep_G = 0.0f;
		}
	}
	else if (colorIndex == 4)
	{
		colorStep_R += colorStepAmount;
		if (colorStep_R >= 1)
		{
			colorIndex++;
			colorStep_R = 1.0f;
		}
	}
	else if (colorIndex == 5)
	{
		colorStep_B -= colorStepAmount;
		if (colorStep_B <= 0)
		{
			colorIndex = 0;
			colorStep_B = 0.0f;
		}
	}
}