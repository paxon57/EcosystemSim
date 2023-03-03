#include "Creature.h"

Creature::Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type):
	colliderIndex(_phys.newCollider(_pos, 50.f, _type)),
	type(_type),
	phys(_phys),
	rotation(0.f)
{
	// Handle global
	amountOfCreatures++;

	phys.addForce(colliderIndex, sf::Vector2f(10.f, 0.f) * 100.f * 60.f);
	
	// Setup rays
	for (size_t i = 0; i < rayAmount; i++)
	{
		ray.emplace_back(Raycast());
	}

	// Setup texture
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

	// Calculate angle between rays
	rayAngDiff = fov / rayAmount;
}

void Creature::update(float deltaTime) {
	// Apply gravity
	phys.addForce(colliderIndex, sf::Vector2f(0.f, 9.81f) * 100.f);

	// Cast rays
	for (int i = 0; i < rayAmount; i++)
	{
		float ang = rotation + (i * rayAngDiff) - (fov / 2.f);
		sf::Vector2f endPos = pos + sf::Vector2f(cos(ang), sin(ang)) * rayLength;
		//phys.raycast(colliderIndex, ray[i], pos, endPos);
	}

	//TEST
	rotation += (PI / 2.f) * deltaTime;
}

void Creature::draw()
{
	pos = phys.getPos(colliderIndex);

	sf::Transform transform;
	//transform.translate(pos);
	transform.rotate((rotation / (2*PI)) * 360.f);

	creaturesQuads[colliderIndex * 4].position = pos + transform.transformPoint(-50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 1].position = pos + transform.transformPoint(50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 2].position = pos + transform.transformPoint(50.f, 50.f);
	creaturesQuads[colliderIndex * 4 + 3].position = pos + transform.transformPoint(-50.f, 50.f);
}
