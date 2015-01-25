#include "Material.h"


Material::Material(void)
	:m_DiffuseColor(1.0f, 1.0f, 1.0f),
	mp_DiffuseMap(nullptr)
{
}


Material::~Material(void)
{
}

void Material::bindTexture()
{
	if(mp_DiffuseMap)
	{
		glActiveTexture(GL_TEXTURE0);
		mp_DiffuseMap->bindThisTexture();
	}
}

