#include "Creature.h"

Creature::Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type):
	colliderIndex(_phys.newCollider(_pos, 50.f, _type)),
	type(_type),
	phys(_phys),
	rotation(0.f),
	net(5, 2)
{	
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

	// Set random rotation
	rotation = ((float)rand() / RAND_MAX) * 2 * PI;

	// Subscribe to collision signal
	phys.colliders[colliderIndex].Collision.connect(&Creature::OnCollision, this);
}

void Creature::update(float deltaTime) {
	// Cast rays
	for (int i = 0; i < rayAmount; i++)
	{
		float ang = rotation + (i * rayAngDiff) - (fov / 2.f);
		sf::Vector2f endPos = pos + sf::Vector2f(cos(ang), sin(ang)) * rayLength;
		//phys.raycast(colliderIndex, ray[i], pos, endPos);
	}
	
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

void Creature::applySettings(CreatureSettings settings) {
	hp = settings.hp;
	dmg = settings.dmg;
	rayAmount = settings.rayAmount;
	rayLength = settings.rayDistance * 100.f;
	fov = (settings.fov / 360.f) * 2 * PI;
}

void Creature::OnCollision(Collider& other)
{
	
}
