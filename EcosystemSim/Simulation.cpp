#include "Simulation.h"

Simulation::Simulation(PhysicsEngine& _phys, CameraController& _cam) :
	phys(_phys),
	cam(_cam)
{
}

void Simulation::update(float dt)
{
	if (!running) imguiSetup();
	else if (running) {
		updateCreatures(dt);
		updateSelection();
		
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

void Simulation::updateSelection()
{
	// Detect middle click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && !lastMiddleClick) {
		bool found = false;

		// Get world mouse pos
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(mousePos);

		// Check if clicked on creature
		for (size_t i = 0; i < creatures.size(); i++) {
			sf::Vector2f diff = pos - creatures[i].pos;
			if (diff.x * diff.x + diff.y * diff.y < 2500.f) {
				// If clicked on creature, select it and enable cam following
				selectedCreature = i;
				found = true;
				camFollowing = true;
				break;
			}
		}
		// If clicked but creature was not found, deselect
		if (!found) {
			selectedCreature = -1;
			camFollowing = false;
		}
	}
	
	// Cancel cam following if user tries to move it
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		camFollowing = false;

	// Check if creature is selected
	if (selectedCreature != -1) {
		// Camera follow
		if (camFollowing) {
			cam.position = creatures[selectedCreature].pos;
			cam.view.setCenter(cam.position);
		}
		// Display creature info
		imguiCreatureInfo(selectedCreature);
	}

	lastMiddleClick = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
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
