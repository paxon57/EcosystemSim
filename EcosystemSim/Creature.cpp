#include "Creature.h"
#include "global.h"

Creature::Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type):
	colliderIndex(-1),
	type(_type),
	phys(_phys)
{
	colliderIndex = phys.newCollider(_pos);

	if (type == CreatureType::Prey) {
		creaturesQuads[colliderIndex * 4].texCoords = sf::Vector2f(0.f, 0.f);
		creaturesQuads[colliderIndex * 4 + 1].texCoords = sf::Vector2f(128.f, 0.f);
		creaturesQuads[colliderIndex * 4 + 2].texCoords = sf::Vector2f(128.f, 128.f);
		creaturesQuads[colliderIndex * 4 + 3].texCoords = sf::Vector2f(0.f, 128.f);
	}
	else {
		creaturesQuads[colliderIndex * 4].texCoords = sf::Vector2f(128.f, 0.f);
		creaturesQuads[colliderIndex * 4 + 1].texCoords = sf::Vector2f(256.f, 0.f);
		creaturesQuads[colliderIndex * 4 + 2].texCoords = sf::Vector2f(256.f, 128.f);
		creaturesQuads[colliderIndex * 4 + 3].texCoords = sf::Vector2f(128.f, 128.f);
	}
}

void Creature::update(float deltaTime) {
	// Get current position from Physics Engine
	pos = phys.getPos(colliderIndex);
	// Move 1m/s down
	phys.move(colliderIndex, pos + sf::Vector2f(0, 100.f) * deltaTime);
}

void Creature::draw()
{
	pos = phys.getPos(colliderIndex);

	creaturesQuads[colliderIndex * 4].position = pos + sf::Vector2f(-50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 1].position = pos + sf::Vector2f(50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 2].position = pos + sf::Vector2f(50.f, 50.f);
	creaturesQuads[colliderIndex * 4 + 3].position = pos + sf::Vector2f(-50.f, 50.f);
}
