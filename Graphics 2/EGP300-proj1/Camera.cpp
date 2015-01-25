#include "Camera.h"


Camera::Camera(void)
{
	m_ViewUpdated = true;
	m_ProjUpdated = true;
	m_DirectionUpdated = true;
	m_Moving = true;
	m_FlyMode = false;
	m_MaxMoveSpeed = 5.0f;
	m_ManualMoveSpeed = 6.0f;
	m_Acceleration = .7f;
	m_CurrentMoveSpeed = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraPos = m_Pos; 

}

void Camera::update()
{
	if(m_FlyMode)
	{
		translateForward(m_CurrentMoveSpeed.x);
		translateRight(m_CurrentMoveSpeed.z);
		translateUp(m_CurrentMoveSpeed.y);
	}
}

glm::mat4x4 Camera::getViewMatrix(void)
{
	if(!m_ViewUpdated && !m_DirectionUpdated)
		return m_CachedView;

	m_CachedView = lookAt(getPos(), getPos() + getDirection(), getUp());	

	m_ViewUpdated = false;
	return m_CachedView;
}

void Camera::translateForward(const float& translateAmount)
{
	m_Pos += getDirection() * translateAmount;

	m_ViewUpdated = true;
}

void Camera::translateBackward(const float& translateAmount)
{
	m_Pos -= getDirection() * translateAmount;

	m_ViewUpdated = true;
}

void Camera::translateLeft(const float& translateAmount)
{
	m_Pos -= getRight() * translateAmount;

	m_ViewUpdated = true;
}

void Camera::translateRight(const float& translateAmount)
{
	m_Pos += getRight() * translateAmount;

	m_ViewUpdated = true;
}

void Camera::translateUp(const float& translateAmount)
{
	m_Pos.y += translateAmount;
	
	m_ViewUpdated = true;
}

void Camera::translateDown(const float& translateAmount)
{
	m_Pos.y -= translateAmount;

	m_ViewUpdated = true;
}

glm::vec3 Camera::getDirection(void)
{
	if(!m_DirectionUpdated)
		return m_Direction;

	//"black magic code"
	m_Direction.x = cos(m_Yaw) * cos(m_Pitch);
	m_Direction.y = sin(m_Pitch);
	m_Direction.z = sin(-m_Yaw) * cos(m_Pitch);

	m_DirectionUpdated = false;

	return m_Direction;
}

void Camera::setDirection(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
	m_Direction.x = x; 
	m_Direction.y = y; 
	m_Direction.z = z;

	m_Pitch = glm::acos(m_Direction.z);
	m_Yaw = atan2f(m_Direction.x, m_Direction.y);

	m_ViewUpdated = true;
}

void Camera::hardReset(int screenWidth, int screenHeight)
{
	setPos(0.0f, 0.0f, 0.0f);
	setDirection(0.0f, 0.0f, 0.0f);
	setUp(0.0f, 1.0f, 0.0f);
	setProjectionMatrix((float)screenWidth, (float)screenHeight);
	setYaw(0);
	setPitch(0);
    m_CurrentMoveSpeed = vec3(0,0,0);
	translateBackward(8.0);
	m_ViewUpdated = true;
	m_DirectionUpdated = true;
}

void Camera::screenShot(const int& screenWidth, const int& screenHeight, const string& filename /*= ""*/)
{
	string tmpFilename = filename;
	if (tmpFilename == "")
	{
		stringstream ss;
		ss << "Screenshot_" << time(0) << ".bmp";

		tmpFilename = ss.str();
	}

	SDL_Surface* pTmp = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth, screenHeight, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	char * pixels = new char [3 * screenWidth * screenHeight];

	glReadPixels(0, 0, screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	for (int i = 0 ; i < screenHeight ; i++)
		std::memcpy( ((char *) pTmp->pixels) + pTmp->pitch * i, pixels + 3 * screenWidth * (screenHeight - i - 1), screenWidth * 3 );

	delete [] pixels;

	SDL_SaveBMP(pTmp, tmpFilename.c_str());

	SDL_FreeSurface(pTmp);
}

void Camera::command(vec3 commandDir)
{
	if( m_FlyMode)
	{
		m_CurrentMoveSpeed += commandDir * m_Acceleration;
		if( m_CurrentMoveSpeed.x > m_MaxMoveSpeed)
			m_CurrentMoveSpeed.x = m_MaxMoveSpeed;

		if( m_CurrentMoveSpeed.y > m_MaxMoveSpeed)
			m_CurrentMoveSpeed.y = m_MaxMoveSpeed;

		if( m_CurrentMoveSpeed.z > m_MaxMoveSpeed)
			m_CurrentMoveSpeed.z = m_MaxMoveSpeed;

		if( m_CurrentMoveSpeed.x < -m_MaxMoveSpeed)
			m_CurrentMoveSpeed.x = -m_MaxMoveSpeed;

		if( m_CurrentMoveSpeed.y < -m_MaxMoveSpeed)
			m_CurrentMoveSpeed.y = -m_MaxMoveSpeed;

		if( m_CurrentMoveSpeed.z < -m_MaxMoveSpeed)
			m_CurrentMoveSpeed.z = -m_MaxMoveSpeed;


	}
	else
	{
		translateForward(commandDir.x * m_ManualMoveSpeed);
		translateRight(commandDir.z * m_ManualMoveSpeed);
		translateUp(commandDir.y * m_ManualMoveSpeed);
	}
}

void Camera::resetCurrentSpeed()
{
	m_CurrentMoveSpeed = vec3(0,0,0);
}



