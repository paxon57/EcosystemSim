#pragma once
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "PhysicsEngine.h"
#include "Creature.h"

class Simulation
{
	public:
		Simulation(PhysicsEngine& _phys);

		int preyAmount = 0;
		int predatorAmount = 0;

		void update(float dt);
		void draw();

		std::vector<Creature> creatures;

	private:
		bool running = false;
		int initialPrey = 0;
		int initialPredator = 0;

		float interval = 0.1f;
		float curInterval = interval;

		PhysicsEngine& phys;

		void imguiStats();
		void imguiSetup();

		void beginSimulation();
};