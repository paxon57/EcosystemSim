#pragma once
#include <SFML/Graphics.hpp>
#include "PhysGridCell.h"

class PhysicsEngine
{
	public:
		// Init engine and set bounds
		PhysicsEngine(float _size);
		
		void update(float dt, int subSteps);
		void addCollider(Collider* pCollider);
		void removeCollider(Collider* pCollider);

	private:
		std::vector<Collider*> colliders;
		PhysGridCell grid[128*128];

		float size;

		void physUpdate(float dt);
		int getCellIndex(float _x, float _y);
		void recalculateCellIndex(Collider* pCollider);
		void keepInBounds(Collider* pCollider);
};
