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
	// Get velocity
	sf::Vector2f velocity = pos - lastPos;
	//printf("Pos: %f, %f\n", pos.x, pos.y);
	//printf("Last Pos: %f, %f\n", lastPos.x, lastPos.y);
	//printf("Velocity: %f, %f\n", velocity.x, velocity.y);
	// Save current position
	lastPos = pos;
	//printf("Last Pos after: %f, %f\n----------------------------\n", lastPos.x, lastPos.y);
	// Perform Verlet integration
	pos = pos + velocity + acceleration * dt * dt;
	// Reset acceleration
	acceleration = sf::Vector2f(0.f, 0.f);
}
