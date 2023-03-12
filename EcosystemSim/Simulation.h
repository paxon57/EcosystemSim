#pragma once
#include "Creature.h"

static class Simulation
{
	public:
		Simulation(PhysicsEngine& _phys);

		int preyAmount = 0;
		int predatorAmount = 0;

		std::vector<Creature> creatures;

		void update(float dt);

	private:
		bool running = false;
		int initialPrey = 0;
		int initialPredator = 0;
		int initialMutations = 1;

		int selectedCreature = -1;

		PhysicsEngine& phys;

		void imguiStats();
		void imguiSetup();
		void imguiCreatureInfo(int _id);
		void updateCreatures(float dt);

		void beginSimulation();
};

