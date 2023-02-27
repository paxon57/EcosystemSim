#include "Collider.h"

Collider::Collider(sf::Vector2f _pos, float _radius) :
	radius(_radius),
	gridIdx(0),
	pos(_pos),
	lastPos(_pos),
	idx(-1) {}

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
	// Perform time corrected Verlet integration
	pos = pos + velocity * (dt/lastdt) + acceleration * ((dt + lastdt)/2.f) * dt;
	// Save last dt
	lastdt = dt;
	// Reset acceleration
	acceleration = sf::Vector2f(0.f, 0.f);
}

void Collider::addDrag(float dt)
{
	//sf::Vector2f force;
	//float speed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
	//force = velocity * (speed / (maxSpeed * dt)) * -1.f;
	addForce(-velocity * 0.5f);
}
