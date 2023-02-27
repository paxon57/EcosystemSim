#include "Creature.h"
#include "global.h"

Creature::Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type):
	colliderIndex(_phys.newCollider(_pos)),
	type(_type),
	phys(_phys)
{
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
	phys.addForce(colliderIndex, sf::Vector2f(0.f, 9.81f) * 100.f);
}

void Creature::draw()
{
	pos = phys.getPos(colliderIndex);

	creaturesQuads[colliderIndex * 4].position = pos + sf::Vector2f(-50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 1].position = pos + sf::Vector2f(50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 2].position = pos + sf::Vector2f(50.f, 50.f);
	creaturesQuads[colliderIndex * 4 + 3].position = pos + sf::Vector2f(-50.f, 50.f);
}
