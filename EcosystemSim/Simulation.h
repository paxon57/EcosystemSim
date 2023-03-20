#pragma once
#include "Creature.h"
#include "CameraController.h"
#include "global.h"

struct CollisionInfo {
	CollisionInfo(int i1, int i2) {
		index1 = i1;
		index2 = i2;
	}
	int index1;
	int index2;
};

static class Simulation
{
	public:
		Simulation(CameraController& _cam);

		int preyAmount = 0;
		int predatorAmount = 0;

		std::vector<int> toRemove;
		std::vector<Creature> creatures;
		std::vector<CollisionInfo> collisions;

		CreatureSettings preySettings;
		CreatureSettings predatorSettings;

		PhysicsEngine phys;

		void update(float dt);
		void OnCollision(int index1, int index2);

	private:
		bool running = false;
		int initialPrey = 500;
		int initialPredator = 500;
		int initialMutations = 0;
		int selectedCreature = -1;
		bool lastClick = false;
		bool camFollowing = false;
		bool collisionHandlingDelay = true;
		float mutationChance = 0.5f;

		CameraController& cam;

		void imguiStats();
		void imguiSetup();
		void imguiCreatureInfo(int _id);
		void updateCreatures(float dt);
		void updateSelection();
		void beginSimulation();
		void processCollisions();
		void removeDeadCreatures();
		void reproduceCreatures();
		void keepInBounds(sf::Vector2f& _pos);
};

