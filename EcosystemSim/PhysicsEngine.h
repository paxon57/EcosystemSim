#pragma once
#include <future>
#include <SFML/Graphics.hpp>
#include "PhysGridCell.h"
#include "Raycast.h"
#include "sigslot/signal.hpp"

const int numThreads = 24;

class PhysicsEngine
{
	public:
		PhysicsEngine(float _size);

		Collider colliders[20000];

		sigslot::signal<int, int> Collision;

		int newCollider(sf::Vector2f _pos, float _radius = 50.f, CreatureType _type = CreatureType::Prey);
		void removeCollider(int index);
		void update(float dt, int subSteps = 1);
		sf::Vector2f getPos(int index);
		void move(int index, sf::Vector2f newPos);
		void addForce(int colliderIndex, sf::Vector2f force);
		void raycast(int senderIndex, Raycast& ray, sf::Vector2f start, sf::Vector2f end);

	private:
		bool firstPass = true;
		float size;
		float deltaTime;

		PhysGridCell grid[128*128];
		std::vector<std::future<void>> futures;

		int getGridIndexFromPos(sf::Vector2f _pos);
		int getGridIndex(int _x, int _y);
		void physUpdate(float dt);
		void physLoop(int x1, int x2, float dt);
		void checkCellCollisions(int _cellIndex, int _otherCellIndex);
		bool collide(int _index, int _otherIndex);
		void solveCollision(int _index, int _otherIndex);
		void keepInBounds(int _index);
		void recalculateIndex(int _index);
		bool indexIsValid(int _index);
};
