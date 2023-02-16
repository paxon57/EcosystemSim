#include "Creature.h"
#include "global.h"

Creature::Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type):
	colliderIndex(-1),
	type(_type),
	phys(_phys)
{
	colliderIndex = phys.newCollider(_pos);
}

void Creature::update(float deltaTime) {
	// Get current position from Physics Engine
	pos = phys.getPos(colliderIndex);
	// Move 1m/s down
	phys.move(colliderIndex, pos + sf::Vector2f(0, 100.f) * deltaTime);
}

void Creature::draw()
{
	sf::CircleShape* body;
	// Get correct body
	if (type == CreatureType::Prey)
		body = &preyBody;
	else
		body = &predatorBody;
	// Set body pos
	(*body).setPosition(phys.getPos(colliderIndex));
	// Draw
	window.draw(*body);
}
