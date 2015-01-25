#pragma once

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Texture.h"

using namespace glm;

class Material
{
public:
	Material(void);
	~Material(void);

	void bindTexture();

	inline vec4 getDiffuseColor( void ) const { return vec4(m_DiffuseColor, 1.0f); }
	inline void setDiffuseColor( const vec4& color ) { m_DiffuseColor = vec3(color); }

	inline bool hasDiffuseMap( void ) const { return (mp_DiffuseMap != nullptr); }
	inline Texture* getDiffuseMap( void ) const { return mp_DiffuseMap; }
	inline void setDiffuseMap( Texture* pTex ) { mp_DiffuseMap = pTex; }

private:
	
	vec3 m_DiffuseColor;
	Texture* mp_DiffuseMap;


};

