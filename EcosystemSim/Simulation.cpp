#include "Simulation.h"

Simulation::Simulation(PhysicsEngine& _phys) :
	phys(_phys)
{
}

void Simulation::update(float dt)
{
	if (running) {
		imguiStats();

		curInterval -= dt;
		if (curInterval < 0.f && initialPrey > 0) {
			curInterval = interval;

			initialPrey -= 10;

			for (size_t i = 0; i < 10; i++)
			{
				Creature& creature = creatures.emplace_back(Creature(phys, sf::Vector2f(1000.f, 500.f + i * 120.f), CreatureType::Prey));
				phys.addForce(creature.colliderIndex, sf::Vector2f(1000.f, 0.f) / dt);
				preyAmount++;
			}
		}

		for (Creature& creature : creatures) {
			creature.update(dt);
		}
	}
	else if (!running) imguiSetup();
}

void Simulation::draw()
{
	for (Creature& creature : creatures) {
		creature.draw();
	}
}

void Simulation::imguiStats()
{
	ImGui::Begin("Simulation Stats");
	ImGui::Value("Prey pop", preyAmount);
	ImGui::Value("Predator pop", predatorAmount);
	ImGui::End();
}

void Simulation::imguiSetup()
{
	ImGui::Begin("Simulation Setup");
	ImGui::Text("Initial population:");
	ImGui::InputInt("Prey", &initialPrey);
	ImGui::InputInt("Predator", &initialPredator);
	if (ImGui::Button("BEGIN SIMULATION")) beginSimulation();
	ImGui::End();
}

void Simulation::beginSimulation()
{
	printf("Starting the sim\n");

	running = true;
}
