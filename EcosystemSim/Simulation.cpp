#include "Simulation.h"

Simulation::Simulation(CameraController& _cam) :
	cam(_cam),
	phys(12800.f)
{
	// Subscribe to collision event
	phys.Collision.connect(&Simulation::OnCollision, this);
}

void Simulation::update(float dt)
{
	if (!running) imguiSetup();
	else if (running) {
		processCollisions();
		removeDeadCreatures();
		reproduceCreatures();
		updateCreatures(dt);
		updateSelection();
		
		if (selectedCreature != -1)
			creatures[selectedCreature].drawRays();

		phys.update(dt, 4);

		imguiStats();

		if (collisionHandlingDelay)
			collisionHandlingDelay = false;
	}
}

// Collision between colliders, collider indexes passed
void Simulation::OnCollision(int index1, int index2)
{
	if (collisionHandlingDelay) return;

	// If same type, skip
	if (phys.colliders[index1].type == phys.colliders[index2].type) return;

	// Avoid duplicates
	for (CollisionInfo info : collisions) {
		if ((info.index1 == index1 && info.index2 == index2) ||
			(info.index2 == index1 && info.index1 == index2))
			return;
	}

	// Add to collisions for processing
	collisions.emplace_back(CollisionInfo(index1, index2));
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
	ImGui::InputFloat("Max Energy", &preySettings.energy);
	ImGui::InputFloat("DMG", &preySettings.dmg);
	ImGui::InputInt("Ray Amount", &preySettings.rayAmount);
	ImGui::InputFloat("Ray Distance", &preySettings.rayDistance);
	ImGui::InputFloat("FOV", &preySettings.fov);
	ImGui::PopItemWidth();
	ImGui::Text("Predator Settings:");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::InputInt("Population ", &initialPredator);
	ImGui::InputFloat("Max Energy ", &predatorSettings.energy);
	ImGui::InputFloat("DMG ", &predatorSettings.dmg);
	ImGui::InputInt("Ray Amount ", &predatorSettings.rayAmount);
	ImGui::InputFloat("Ray Distance ", &predatorSettings.rayDistance);
	ImGui::InputFloat("FOV ", &predatorSettings.fov);
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

	ImGui::Value("Energy", creatures[_id].energy);

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

void Simulation::processCollisions()
{
	for (CollisionInfo info : collisions) {
		// Find creatures involved based on collider index
		Creature* c1 = nullptr;
		int i1 = 0;
		Creature* c2 = nullptr;
		int i2 = 0;
		for (int i = 0; i < creatures.size(); i++) {
			if (creatures[i].colliderIndex == info.index1) {
				c1 = &creatures[i];
				i1 = i;
			}
			if (creatures[i].colliderIndex == info.index2) {
				c2 = &creatures[i];
				i2 = i;
			}
			
			if (c1 != nullptr && c2 != nullptr)
				break;
		}

		// Return if not found for any reason
		if (c1 == nullptr || c2 == nullptr) continue;

		// Apply dmg
		c1->energy -= c2->dmg;
		c2->energy -= c1->dmg;

		// Add toRemove if dead and call OnKill on the killer
		if (c1->energy <= 0) {
			toRemove.emplace_back(i1);
			c2->OnKill();
		}
		if (c2->energy <= 0) {
			toRemove.emplace_back(i2);
			c1->OnKill();
		}
	}

	collisions.clear();
}

void Simulation::removeDeadCreatures()
{
	std::vector<int> removed;
	for (int i : toRemove)
	{
		if (i >= creatures.size()) continue;

		// Check if not already removed
		bool duplicate = false;
		for (int j : removed)
			if (i == j) duplicate = true;

		if (duplicate) continue;

		// Decrease stats
		if (creatures[i].type == CreatureType::Prey)
			preyAmount--;
		else
			predatorAmount--;

		// Deselect creature if selected
		if (selectedCreature == i) selectedCreature = -1;

		// Move selection if selected is last
		if (selectedCreature == creatures.size() - 1) selectedCreature = i;

		// Remove
		creatures[i].death();
		creatures[i] = creatures.back();
		creatures.pop_back();
		removed.emplace_back(i);

	}
	toRemove.clear();
}

void Simulation::reproduceCreatures()
{
	for (int i = 0; i < creatures.size(); i++) {
		if (creatures[i].canReproduce) {
			// Reproduction
			int r = rand();
			sf::Vector2f pos = creatures[i].pos + sf::Vector2f(sinf(r), cosf(r)) * 100.f;
			keepInBounds(pos);
			Creature& c = creatures.emplace_back(Creature(phys, pos, creatures[i].type));

			if (creatures[i].type == CreatureType::Prey) {
				c.applySettings(preySettings);
				preyAmount++;
			}
			else {
				c.applySettings(predatorSettings);
				predatorAmount++;
			}

			c.net = creatures[i].net;
			c.net.mutate(); // TODO Add mutation chance

			creatures[i].canReproduce = false;
		}

		// No energy death
		if (creatures[i].energy < 0.f)
			toRemove.emplace_back(i);
	}
}

void Simulation::keepInBounds(sf::Vector2f& pos)
{
	if (pos.x < 50.f) pos.x = 50.f;
	if (pos.x > 12750.f) pos.x = 12750.f;
	if (pos.y < 50.f) pos.y = 50.f;
	if (pos.y > 12750.f) pos.y = 12750.f;
}
