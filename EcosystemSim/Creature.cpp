#include "Creature.h"

Creature::Creature(CreatureType _type):
collider(&pos, 50.f) {
	type = _type;
	phys.init(&collider);
}

void Creature::update(float deltaTime) {

	sf::CircleShape* body;
	// Get correct body
	if (type == CreatureType::Prey)
		body = &preyBody;
	else
		body = &predatorBody;

	// Move
	pos = phys.move(collider, sf::Vector2f(0.f, 5.f));

	// Set body pos
	(*body).setPosition(pos);

	// Draw
	window.draw(*body);
}

Creature::~Creature()
{
	phys.remove(&collider);
}
