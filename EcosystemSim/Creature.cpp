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
	sf::Vector2f nextMove = lastMove + sf::Vector2f((rand() / (float)RAND_MAX) * 1.f - 0.5f, (rand() / (float)RAND_MAX) * 1.f - 0.5f);
	move(nextMove * 10.f * deltaTime);
	lastMove = nextMove;

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
