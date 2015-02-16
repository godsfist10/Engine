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

}