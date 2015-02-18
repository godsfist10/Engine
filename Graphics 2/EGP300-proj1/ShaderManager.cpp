#include "ShaderManager.h"


Shader_Manager::Shader_Manager(void)
{
	initAll();
}


Shader_Manager::~Shader_Manager(void)
{

}

void Shader_Manager::cleanup()
{
	for (auto it = mShaderMap.itBegin(); it != mShaderMap.itEnd(); ++it)
	{
		Shader* pShader = it->second;
		delete pShader;
	}
	mShaderMap.clear();

}


void Shader_Manager::initAll()   //when you add a new shader add it here in the format below
{
	/*
	ShaderClass* temp = new ShaderClass();
	temp->init();
	addShader("desiredNameToCall", temp);
	*/

	WaveShader* wave = new WaveShader();
	wave->init();
	addShader("waveShader", wave);

	ColorBombShader* bomb = new ColorBombShader();
	bomb->init();
	addShader("colorShiftShader", bomb);

	Diffuse* diffuse = new Diffuse();
	diffuse->init();
	addShader("diffuse", diffuse);

}

void Shader_Manager::addShader(const string& shaderName, Shader* shaderToAdd)
{
	mShaderMap.add(shaderName, shaderToAdd);
}

void Shader_Manager::setupForDraw(const string& shaderName,mat4x4 mvpMatrix)
{
	if (hasShader(shaderName))
	{
		mShaderMap[shaderName]->setupForDraw(mvpMatrix);
	}
}

void Shader_Manager::update(double deltaTime)
{
	for (auto it = mShaderMap.itBegin(); it != mShaderMap.itEnd(); ++it)
	{
		it->second->update(deltaTime);
	}
}
