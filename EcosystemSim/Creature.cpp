#include "Creature.h"

Creature::Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type):
	colliderIndex(_phys.newCollider(_pos, 50.f, _type)),
	type(_type),
	phys(&_phys),
	rotation(0.f),
	net(1, 1)
{	
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
}

void Creature::update(float deltaTime) {
	
	manageEnergy(deltaTime);

	// Cast rays
	for (int i = 0; i < rayAmount; i++)
	{
		float ang = rotation + (i * rayAngDiff) - (fov / 2.f) - PI/2.f;
		sf::Vector2f endPos = pos + sf::Vector2f(cos(ang), sin(ang)) * rayLength;
		phys->raycast(colliderIndex, ray[i], pos, endPos);
	}

	// Prepare inputs 
	// [0] - ray 0 dist
	// [1] - type hit: -1 = friend hit, 0 = no hit, 1 = enemy hit
	// ... repeat for all rays ...
	// [Last] bias = 1
	std::vector<float> inputs(rayAmount * 2 + 1);
	
	for (int i = 0; i < rayAmount; i++) {
		inputs[i * 2] = ray[i].distance / rayLength;
		inputs[i * 2 + 1] = ray[i].hit ? ((ray[i].prey && type == CreatureType::Predator) || (!ray[i].prey && type == CreatureType::Prey) ? 1.f : -1.f) : 0.f;
	}
	inputs[inputs.size() - 1] = 1.f; // Bias

	// Run network
	net.setInputs(inputs);
	net.run();
	std::vector<float> outputs = net.getOutputs(); // Outputs range from -2 to 2

	// Apply forward force and use energy to do it, 5 units per sec at full speed
	forwardForce(outputs[0] * 300.f);
	energy -= fabs(outputs[0]) * 2.5f * deltaTime;

	// Apply rotation
	rotation += outputs[1] * 1.5f * deltaTime;
}

void Creature::draw()
{
	pos = phys->getPos(colliderIndex);

	sf::Transform transform;
	//transform.translate(pos);
	transform.rotate((rotation / (2*PI)) * 360.f);

	creaturesQuads[colliderIndex * 4].position = pos + transform.transformPoint(-50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 1].position = pos + transform.transformPoint(50.f, -50.f);
	creaturesQuads[colliderIndex * 4 + 2].position = pos + transform.transformPoint(50.f, 50.f);
	creaturesQuads[colliderIndex * 4 + 3].position = pos + transform.transformPoint(-50.f, 50.f);
}

void Creature::death()
{
	// Remove collider
	phys->removeCollider(colliderIndex);

	// Remove graphics
	creaturesQuads[colliderIndex * 4].position = sf::Vector2f(0.f, 0.f);
	creaturesQuads[colliderIndex * 4 + 1].position = sf::Vector2f(0.f, 0.f);
	creaturesQuads[colliderIndex * 4 + 2].position = sf::Vector2f(0.f, 0.f);
	creaturesQuads[colliderIndex * 4 + 3].position = sf::Vector2f(0.f, 0.f);
}

void Creature::drawRays()
{
	// Draw rays
	for (int i = 0; i < rayAmount; i++)
	{
		float ang = rotation + (i * rayAngDiff) - (fov / 2.f) - PI/2.f;
		sf::Vector2f endPos = pos + sf::Vector2f(cos(ang), sin(ang)) * ray[i].distance;
		lines[i * 2].position = pos;
		lines[i * 2 + 1].position = endPos;
	}
}

void Creature::OnKill() {
	if (type == CreatureType::Predator) {
		energy += maxEnergy * 0.75f;
	}
}

void Creature::forwardForce(float _force)
{
	sf::Vector2f force(sinf(rotation) * _force, -cosf(rotation) * _force);
	phys->addForce(colliderIndex, force);
}

void Creature::manageEnergy(float dt)
{
	// Gain 4% energy per sec as Prey
	// Lose 2% energy per sec as Predator
	if (type == CreatureType::Prey)
		energy += (maxEnergy / 25.f) * dt;
	else 
		energy -= (maxEnergy / 50.f) * dt;

	// Reproduce if energy full, use half of it
	if (energy > maxEnergy) {
		canReproduce = true;
		energy = maxEnergy / 2.f;
	}
}

void Creature::applySettings(CreatureSettings settings) {
	dmg = settings.dmg;
	rayAmount = settings.rayAmount;
	rayLength = settings.rayDistance * 100.f;
	fov = (settings.fov / 360.f) * 2 * PI;
	maxEnergy = settings.energy;
	energy = maxEnergy * 0.75f;

	// Setup rays
	for (size_t i = 0; i < rayAmount; i++)
	{
		ray.emplace_back(Raycast());
	}

	net = NEAT(rayAmount * 2 + 1, 2);
	rayAngDiff = fov / rayAmount;
}

