#pragma once
#define GLM_FORCE_RADIANS

#include "ResourceManager.h"
#include <vector>
#undef main


using namespace glm;

class Skybox : public Object
{
public:

	Skybox(void);
	Skybox(const string& textureFilename, ResourceManager* resourceManager, const GLfloat& scale);
	Skybox(const string& textureFilename, ResourceManager* resourceManager, const GLfloat& scale, const string& objectName);
	~Skybox();
	virtual void draw(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, GLShaderManager& shaderManager);
	void loadSkybox(const string& texFilename, ResourceManager* resourceManager, const GLfloat& scale);

private:

	inline void clearDepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT); }

};

