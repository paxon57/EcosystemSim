#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(PhysicsEngine& _phys, int _colliderIndex, sf::Vector2f _pos) :
	phys(_phys),
	colliderIndex(_colliderIndex),
	pos(_pos),
	lastPos(_pos)
{
}

void PhysicsObject::addForce(sf::Vector2f force)
{
	acceleration += force;
}

void PhysicsObject::update(float dt)
{
	// Get current position from Physics Engine
	pos = phys.getPos(colliderIndex);
	// Get velocity
	sf::Vector2f velocity = pos - lastPos;
	// Save current position
	lastPos = pos;
	// Perform Verlet integration
	phys.move(colliderIndex, pos + velocity + acceleration * dt * dt);
	// Reset acceleration
	acceleration = sf::Vector2f(0.f, 0.f);
}
