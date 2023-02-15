#include "Creature.h"
#include "global.h"

Creature::Creature(CreatureType _type):
	collider(Collider(pos)),
	type(_type) {}

void Creature::update(float deltaTime) {

	sf::CircleShape* body;
	// Get correct body
	if (type == CreatureType::Prey)
		body = &preyBody;
	else
		body = &predatorBody;

	// Move 1m/s down
	move(sf::Vector2f(0.f, 100.f * deltaTime));

	// Set body pos
	(*body).setPosition(pos);

	// Draw
	window.draw(*body);
}

void Creature::move(sf::Vector2f move)
{
	pos = pos + move;
}

Creature::~Creature()
{
	
}
