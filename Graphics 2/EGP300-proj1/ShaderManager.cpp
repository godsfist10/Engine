#include "ShaderManager.h"


Shader_Manager::Shader_Manager(void)
{

}


Shader_Manager::~Shader_Manager(void)
{

}

void Shader_Manager::init(const string& vertexProgramName, const string& fragmentProgramName)
{
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

void Shader_Manager::setupForDraw(const string& shaderName,mat4x4 mvpMatrix)
{
	float colorWater[] = {0.145f, 0.427f, 0.482f, 0.8f}; 

	glUseProgram(waterShader); 
	glUniform1i(baseImageLoc, 0);
	glUniform1f(locTime, waveTime); 
	glUniform1f(locWaveWidth, waveWidth); 
	glUniform1f(locWaveHeight, waveHeight); 
	glUniform4fv(locColor, 1, colorWater); 
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]); 

}

void Shader_Manager::update()
{
	float currentTime = (float)glutGet(GLUT_ELAPSED_TIME); 
	float deltaTime = ((float)currentTime - lastTime) / 1000; 
	lastTime = currentTime; 

	waveTime += deltaTime; 
}
