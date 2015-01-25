#pragma once

#define GLM_FORCE_RADIANS
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

using namespace glm;

class Rectangle_3D
{
public:


	inline Rectangle_3D::Rectangle_3D(vec3 pos, vec3 dimensions)
		:m_Pos(pos)
		,m_Dimensions(dimensions)
	{};

	inline Rectangle_3D(void)
		:m_Pos(vec3(0,0,0)),
		m_Dimensions(vec3(0,0,0))
	{};
	
	~Rectangle_3D(void);
	
	glm::vec3 Pos() const { return m_Pos; }
	void Pos(glm::vec3 val) { m_Pos = val; }

	glm::vec3 Dimensions() const { return m_Dimensions; }
	void Dimensions(glm::vec3 val) { m_Dimensions = val; }

protected:

	vec3 m_Pos;
	vec3 m_Dimensions; //width, height, depth z
	


};

