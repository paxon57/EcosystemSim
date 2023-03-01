#pragma once
#include "PhysicsEngine.h"

class Raycast
{
	public:
		Raycast(PhysicsEngine& _phys);

		bool hit = false;
		bool hitWorld = false;
		int targetIndex = -1;
		float distance;

		void shoot(sf::Vector2f startPoint, sf::Vector2f endPoint);
	private:
		PhysicsEngine& phys;
};

