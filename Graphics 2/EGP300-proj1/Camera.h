#pragma once

//big help from Stephen / based code off from him

#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#define GLM_FORCE_RADIANS
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

#include <string>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#undef main

#include "Vector3D.h"

using namespace glm;
using namespace std;



class Camera
{
public:
	Camera(void);
	inline ~Camera(void){ }

	mat4x4 getViewMatrix( void );
	void hardReset(int screenWidth, int screenHeight);
	void translateForward( const float& translateAmount );
	void translateBackward( const float& translateAmount );
	void translateLeft(const float& translateAmount );
	void translateRight( const float& translateAmount );
	void translateUp(const float& translateAmount);
	void translateDown( const float& translateAmount);
	void update();
	void screenShot(const int& screenWidth, const int& screenHeight, const string& filename = "");
	
#pragma region getters and setters

	inline bool getMoving() const { return m_Moving; }
	inline void setMoving(bool val) { m_Moving = val; }

	inline vec3 getPos( void ) const { return m_Pos; }
	inline void setPos( const vec3& pos ) { setPos(pos.x, pos.y, pos.z); }
	inline void setPos( const GLfloat& x, const GLfloat& y, const GLfloat& z ) { m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; m_ViewUpdated = true; }

	vec3 getDirection( void );
	inline void setDirection( const vec3& dir ) { setDirection(dir.x, dir.y, dir.z); }
	void setDirection( const GLfloat& x, const GLfloat& y, const GLfloat& z );
	
	inline vec3 getUp( void ) const { return m_Up; }
	inline void setUp( const vec3& up ) { setUp(up.x, up.y, up.z); }
	inline void setUp( const GLfloat& x, const GLfloat& y, const GLfloat& z ) { m_Up.x = x; m_Up.y = y; m_Up.z = z; m_ViewUpdated = true; }

	inline GLfloat getNear() const { return m_Near; }
	inline void setNear(GLfloat nearq) { m_Near = nearq; }

	inline GLfloat getFar() const { return m_Far; }
	inline void setFar(GLfloat farq) { m_Far = farq; }

	inline GLfloat getYaw() const { return m_Yaw; }
	inline void setYaw(GLfloat val) { m_Yaw = val; m_DirectionUpdated = true; }

	inline GLfloat getPitch() const { return m_Pitch; }
	inline void setPitch(GLfloat val) { m_Pitch = val; m_DirectionUpdated = true; }

	inline vec3 getLeft( void ) { return cross(m_Up, getDirection()); }
	inline vec3 getRight( void ) { return cross(getDirection(), m_Up); }

	inline mat4x4 getProjectionMatrix() const { return m_CachedProjection;}
	inline void setProjectionMatrix(float width, float height ) { m_CachedProjection = perspectiveFov(45.0f, width, height, 1.0f, 10000.0f); }

	bool FlyMode() const { return m_FlyMode; }
	void FlyMode(bool val) { m_FlyMode = val; resetCurrentSpeed(); }

	void resetCurrentSpeed();	
	void command(vec3 commandDir);

#pragma endregion

protected:

	vec3 m_Pos, m_Direction, m_Up;
	vec3 forwardSideUpBool;

	mat4x4 m_CachedView, m_CachedProjection;

	GLfloat m_Near, m_Far, m_Pitch, m_Yaw;
	
	bool m_ViewUpdated, m_ProjUpdated, m_DirectionUpdated;

	bool m_Moving;
	
	vec3 m_CurrentMoveSpeed;
	float m_ManualMoveSpeed;
	float m_MaxMoveSpeed;
	float m_Acceleration;
	float m_AccelerationRamp;
	bool m_FlyMode;
	
	float getMagnitude(vec3 dir);
	vec3 normalized(vec3 dir);
	


};

