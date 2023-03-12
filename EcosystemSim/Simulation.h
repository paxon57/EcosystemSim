#pragma once
#include "Creature.h"
#include "CameraController.h"
#include "global.h"

static class Simulation
{
	public:
		Simulation(PhysicsEngine& _phys, CameraController& _cam);

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
		bool lastMiddleClick = false;
		bool camFollowing = false;

		PhysicsEngine& phys;
		CameraController& cam;

		void imguiStats();
		void imguiSetup();
		void imguiCreatureInfo(int _id);
		void updateCreatures(float dt);
		void updateSelection();

		void beginSimulation();
};

