#pragma once
#include "PhysicsEngine.h"

class PhysicsObject
{
	public:
		PhysicsObject(PhysicsEngine& _phys, int _colliderIndex, sf::Vector2f _pos);

		void addForce(sf::Vector2f force);

		void update(float dt);

		sf::Vector2f pos;
		sf::Vector2f lastPos;

	private:
		int colliderIndex;

		
		sf::Vector2f acceleration;

		PhysicsEngine& phys;
};

