#include "ShaderManager.h"


Shader_Manager::Shader_Manager(void)
{
	waterShaderInUse = false;
	colorBombShaderInUse = false;
}


Shader_Manager::~Shader_Manager(void)
{

}

void Shader_Manager::init(const string& vertexProgramName, const string& fragmentProgramName)
{
	if (vertexProgramName == "WaterVertShader.vp")
	{
		waterShaderInUse = true;
		waveTime = 0.0f;
		waveWidth = 0.03f;
		waveHeight = 3.0f;

		waterShader = gltLoadShaderPairWithAttributes("WaterVertShader.vp", "WaterFragShader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "texCoord");

		locColor = glGetUniformLocation(waterShader, "vColorValue");
		locMVP = glGetUniformLocation(waterShader, "mvpMatrix");
		locTime = glGetUniformLocation(waterShader, "waveTime");
		locWaveWidth = glGetUniformLocation(waterShader, "waveWidth");
		locWaveHeight = glGetUniformLocation(waterShader, "waveHeight");
		baseImageLoc = glGetUniformLocation(waterShader, "baseImage");
	}
	if (vertexProgramName == "ColorVertShader.vp")
	{
		colorBombShaderInUse = true;

		colorStepAmount = .05f;
		colorIndex = 0;

		colorStep_R = 1;
		colorStep_G = 0;
		colorStep_B = 0;

		colorBombShader = gltLoadShaderPairWithAttributes("ColorVertShader.vp", "ColorFragShader.fp", 1, GLT_ATTRIBUTE_VERTEX, "vVertex");

		locColorBomb = glGetUniformLocation(colorBombShader, "vColorBombValue");
		locMVP_Bomb = glGetUniformLocation(colorBombShader, "mvpMatrix");

	}
}

void Shader_Manager::setupForDraw(const string& shaderName,mat4x4 mvpMatrix)
{

	if (shaderName == "waterShader")
	{
		float colorWater[] = { 0.145f, 0.427f, 0.482f, 0.8f };

		glUseProgram(waterShader);
		glUniform1i(baseImageLoc, 0);
		glUniform1f(locTime, waveTime);
		glUniform1f(locWaveWidth, waveWidth);
		glUniform1f(locWaveHeight, waveHeight);
		glUniform4fv(locColor, 1, colorWater);
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
	}
	if (shaderName == "colorBombShader")
	{
		float colorBomb[] = { colorStep_R, colorStep_G, colorStep_B, 1.0f };
		
		glUseProgram(colorBombShader);
		glUniform4fv(locColorBomb, 1, colorBomb);
		glUniformMatrix4fv(locMVP_Bomb, 1, GL_FALSE, &mvpMatrix[0][0]);
	}
}

void Shader_Manager::update(double deltaTime)
{
	if (waterShaderInUse)
	{
		waveTime += (float)deltaTime;
	}

	if (colorBombShaderInUse)
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

}
