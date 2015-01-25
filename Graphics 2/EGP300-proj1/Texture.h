#pragma once

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>


class Texture
{
public:
	Texture(GLuint textureID);
	~Texture(void);

	void bindThisTexture();

#pragma region getters and setters

	inline GLuint getTextureID() const {return m_TextureID;}
	inline void setTextureID(const GLuint& texID) {m_TextureID = texID;} 

#pragma endregion

private:

	GLuint m_TextureID;

};

