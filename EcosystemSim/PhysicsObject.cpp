#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(PhysicsEngine& _phys, int _colliderIndex, sf::Vector2f _pos) :
	phys(_phys),
	colliderIndex(_colliderIndex),
	pos(_pos.x, _pos.y),
	lastPos(_pos.x, _pos.y)
{
}

void PhysicsObject::addForce(sf::Vector2f force)
{
	acceleration += force;
	//printf("TEST: %f, %f\n", lastPos.x, lastPos.y);
}

void PhysicsObject::update(float dt)
{
	// Get current position from Physics Engine
	pos = phys.getPos(colliderIndex);
	// Get velocity
	sf::Vector2f velocity = pos - lastPos;
	printf("Pos: %f, %f\n", pos.x, pos.y);
	printf("Last Pos: %f, %f\n", lastPos.x, lastPos.y);
	printf("Velocity: %f, %f\n", velocity.x, velocity.y);
	// Save current position
	lastPos = pos;
	printf("Last Pos after: %f, %f\n----------------------------\n", lastPos.x, lastPos.y);
	// Perform Verlet integration
	phys.move(colliderIndex, pos + velocity + acceleration * dt * dt);
	// Reset acceleration
	acceleration = sf::Vector2f(0.f, 0.f);
}
