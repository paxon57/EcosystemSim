#include "Simulation.h"

Simulation::Simulation(PhysicsEngine& _phys) :
	phys(_phys)
{
}

void Simulation::update(float dt)
{
	selectedCreature = 0; ////////////////////////
	if (!running) imguiSetup();
	else if (running) {
		updateCreatures(dt);

		if (selectedCreature != -1)
			imguiCreatureInfo(selectedCreature);
		imguiStats();
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
	ImGui::Text("Initial Population:");
	ImGui::InputInt("Prey", &initialPrey);
	ImGui::InputInt("Predator", &initialPredator);
	ImGui::Text("Initial Mutations:");
	ImGui::SliderInt("##", &initialMutations, 1, 10);
	if (ImGui::Button("BEGIN SIMULATION")) beginSimulation();
	ImGui::End();
}

void Simulation::imguiCreatureInfo(int _id)
{
	ImGui::Begin("Creature Information");

	ImGui::Value("Creature ID", _id);
	if (creatures[_id].type == CreatureType::Predator)
		ImGui::Text("Creature Type: Predator");
	else
		ImGui::Text("Creature Type: Prey");

	ImGui::Text("Network: ");
	creatures[_id].net.draw_net();
	ImGui::End();
}

void Simulation::updateCreatures(float dt)
{
	// Update creatures
	for (Creature& creature : creatures) {
		creature.update(dt);
	}

	// Draw creatures
	for (Creature& creature : creatures) {
		creature.draw();
	}
}

void Simulation::beginSimulation()
{
	// Spawn Prey at random pos
	for (size_t i = 0; i < initialPrey; i++)
	{
		float x = ((float)rand() / RAND_MAX) * 12800.f;
		float y = ((float)rand() / RAND_MAX) * 12800.f;

		Creature& creature = creatures.emplace_back(Creature(phys, sf::Vector2f(x, y), CreatureType::Prey));
		// Mutate
		for (size_t j = 0; j < initialMutations; j++)
		{
			creature.net.mutate();
		}
		preyAmount++;
	}

	// Spawn Predators at random pos
	for (size_t i = 0; i < initialPredator; i++)
	{
		float x = ((float)rand() / RAND_MAX) * 12800.f;
		float y = ((float)rand() / RAND_MAX) * 12800.f;

		Creature& creature = creatures.emplace_back(Creature(phys, sf::Vector2f(x, y), CreatureType::Predator));
		// Mutate
		for (size_t j = 0; j < initialMutations; j++)
		{
			creature.net.mutate();
		}
		predatorAmount++;
	}

	running = true;
}
