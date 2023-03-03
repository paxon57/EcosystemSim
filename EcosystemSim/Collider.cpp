#include "Collider.h"

Collider::Collider(sf::Vector2f _pos, float _radius, CreatureType _type) :
	radius(_radius),
	gridIdx(0),
	pos(_pos),
	lastPos(_pos),
	idx(-1),
	type(_type) {}

void Collider::addForce(sf::Vector2f force)
{
	acceleration += force;
}

void Collider::update(float dt)
{
	// Update velocity
	velocity = pos - lastPos;
	// Add drag
	addDrag(dt);
	// Save current position
	lastPos = pos;
	// Perform Verlet integration
	pos = pos + velocity + acceleration * dt * dt;
	// Reset acceleration
	acceleration = sf::Vector2f(0.f, 0.f);
}

void Collider::addDrag(float dt)
{
	addForce(-0.5f * sf::Vector2f(velocity.x * velocity.x, velocity.y * velocity.y) * drag);
}
