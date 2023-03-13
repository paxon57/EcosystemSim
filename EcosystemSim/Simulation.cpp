#include "Simulation.h"

Simulation::Simulation(CameraController& _cam) :
	cam(_cam),
	phys(12800.f)
{
}

void Simulation::update(float dt)
{
	if (!running) imguiSetup();
	else if (running) {
		updateCreatures(dt);
		updateSelection();
		
		phys.update(dt, 4);

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
	ImGui::Text("Prey Settings:");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::InputInt("Population", &initialPrey);
	ImGui::InputInt("HP", &preySettings.hp);
	ImGui::InputInt("DMG", &preySettings.dmg);
	ImGui::InputInt("Ray Amount", &preySettings.rayAmount);
	ImGui::InputFloat("Ray Distance", &preySettings.rayDistance);
	ImGui::InputFloat("FOV", &preySettings.fov);
	ImGui::PopItemWidth();
	ImGui::Text("Predator Settings:");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::InputInt("Population", &initialPredator);
	ImGui::InputInt("HP", &predatorSettings.hp);
	ImGui::InputInt("DMG", &predatorSettings.dmg);
	ImGui::InputInt("Ray Amount", &predatorSettings.rayAmount);
	ImGui::InputFloat("Ray Distance", &predatorSettings.rayDistance);
	ImGui::InputFloat("FOV", &predatorSettings.fov);
	ImGui::PopItemWidth();
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lastClick && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
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

	lastClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void Simulation::beginSimulation()
{
	// Spawn Prey at random pos
	for (size_t i = 0; i < initialPrey; i++)
	{
		float x = ((float)rand() / RAND_MAX) * 12800.f;
		float y = ((float)rand() / RAND_MAX) * 12800.f;

		Creature& creature = creatures.emplace_back(Creature(phys, sf::Vector2f(x, y), CreatureType::Prey));
		creature.applySettings(preySettings);
		
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
		creature.applySettings(predatorSettings);
		
		// Mutate
		for (size_t j = 0; j < initialMutations; j++)
		{
			creature.net.mutate();
		}
		predatorAmount++;
	}

	running = true;
}
