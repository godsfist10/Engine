#include "Shader.h"


Shader::Shader()
{

}

Shader::~Shader()
{
}

void Shader::update(double deltaTime)
{

}

void Shader::init(const string& vertShaderName, const string& fragShaderName, bool vertexAtt, bool textureAtt)
{
	if (!vertexAtt && !textureAtt)
		mShaderIndex = gltLoadShaderPairWithAttributes(("Assets/Shaders/" + vertShaderName).c_str(), fragShaderName.c_str());
	else if (vertexAtt && !textureAtt)
		mShaderIndex = gltLoadShaderPairWithAttributes(vertShaderName.c_str(), fragShaderName.c_str(), 1, GLT_ATTRIBUTE_VERTEX, "vVertex");
	else if (vertexAtt && textureAtt)
		mShaderIndex = gltLoadShaderPairWithAttributes(vertShaderName.c_str(), fragShaderName.c_str(), 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "texCoord");
	else if (!vertexAtt && textureAtt)
		mShaderIndex = gltLoadShaderPairWithAttributes(vertShaderName.c_str(), fragShaderName.c_str(), 1, GLT_ATTRIBUTE_TEXTURE0, "texCoord");

	locMVP = glGetUniformLocation(mShaderIndex, "mvpMatrix");
}