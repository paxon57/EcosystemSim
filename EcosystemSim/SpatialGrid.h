#pragma once
#include "global.h"
#include "SpatialGridCell.h"

class SpatialGrid
{
	public:
		SpatialGrid(float worldSize = 100.f);
		void move(CircleCollider* collider, sf::Vector2f pos);
		std::vector<CircleCollider*> getColliders(sf::Vector2f pos, float radius);

	private:
		float cellSize = 0.f;

		SpatialGridCell grid[100];
		int getIndex(float x, float y);
};

