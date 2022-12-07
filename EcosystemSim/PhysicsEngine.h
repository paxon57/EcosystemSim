#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "SpatialGrid.h"

class PhysicsEngine
{
	public:
		PhysicsEngine(float mapSize);

		float worldLimit;

		sf::Vector2f move(CircleCollider* mover, sf::Vector2f moveBy);
		
		void init(CircleCollider* collider);
		void remove(CircleCollider* collider);

	private:
		SpatialGrid spatialGrid;

		std::vector<CircleCollider*> colliders;
};