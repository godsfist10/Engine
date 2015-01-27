#include "PhysicsObject.h"


PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(const Map<string, Model*> &modelsMap)
:Object(modelsMap)
{
	m_Velocity = vec3(0, 0, 0); 
	m_Acceleration = vec3(0, 0, 0);
}


PhysicsObject::~PhysicsObject()
{
}


void PhysicsObject::update()
{
	physicsUpdate();
}


void PhysicsObject::physicsUpdate()
{
	modifyVelocity(m_Acceleration);
	Translate(m_Velocity.x, m_Velocity.y, m_Velocity.z);
}