#include "Texture.h"


Texture::Texture(GLuint textureID)
	:m_TextureID(textureID)
{
}


Texture::~Texture(void)
{
	if(m_TextureID)
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
	}
}

void Texture::bindThisTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}
