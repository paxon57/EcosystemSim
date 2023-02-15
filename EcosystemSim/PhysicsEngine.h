#pragma once
#include <SFML/Graphics.hpp>
#include "PhysGridCell.h"

class PhysicsEngine
{
	public:
		PhysicsEngine(float _size);
		int newCollider(sf::Vector2f _pos, float _radius = 50.f);
		void removeCollider(int index);
		void update(float dt, int subSteps = 1);
		sf::Vector2f* getPosPointer(int index);
		void move(int index, sf::Vector2f newPos);

	private:
		float size;

		Collider colliders[20000];
		PhysGridCell grid[128*128];

		int getGridIndexFromPos(sf::Vector2f _pos);
		int getGridIndex(int _x, int _y);
		void physUpdate(float dt);
		void checkCellCollisions(int _cellIndex, int _otherCellIndex);
		bool collide(int _index, int _otherIndex);
		void solveCollision(int _index, int _otherIndex);
		void keepInBounds(int _index);
};
