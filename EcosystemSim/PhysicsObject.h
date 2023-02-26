#pragma once
#include "PhysicsEngine.h"

class PhysicsObject
{
	public:
		PhysicsObject(PhysicsEngine& _phys, int _colliderIndex, sf::Vector2f _pos);

		void addForce(sf::Vector2f force);

		void update(float dt);

	private:
		int colliderIndex;

		sf::Vector2f pos;
		sf::Vector2f lastPos;
		sf::Vector2f acceleration;

		PhysicsEngine& phys;
};

