#pragma once
#include "Shader.h"

class Diffuse :
	public Shader
{
public:
	Diffuse();
	~Diffuse();

	void init();
	void setupForDraw(mat4x4 mvpMatrix);

protected:

	GLuint baseImageLoc;
};

