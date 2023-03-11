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

	// Test
	net.mutate();
}

void Creature::update(float deltaTime) {
	// Cast rays
	for (int i = 0; i < rayAmount; i++)
	{
		float ang = rotation + (i * rayAngDiff) - (fov / 2.f);
		sf::Vector2f endPos = pos + sf::Vector2f(cos(ang), sin(ang)) * rayLength;
		//phys.raycast(colliderIndex, ray[i], pos, endPos);
	}

	//TEST
	net.setInputs(std::vector<float>{0.5f, 0.4f, 0.3f, 0.2f, -1.f});
	net.run();
	std::vector<float> outputs = net.getOutputs();
	printf("-----------------------------------\n");
	for (float val : outputs)
	{
		printf("%f\n", val);
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
