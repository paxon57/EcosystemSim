#pragma once
#include <SFML/Graphics.hpp>
#include "global.h"
#include "Collider.h"
#include "PhysicsEngine.h"
#include "PhysicsObject.h"

enum CreatureType { Prey, Predator};

class Creature
{
	public:
		CreatureType type;

		Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type = CreatureType::Prey);
		void update(float deltaTime);
		void draw();

	private:
		int colliderIndex;
		sf::Vector2f pos;
		sf::Vector2f lastMove;
		PhysicsEngine& phys;
		PhysicsObject po;

		void addForce(sf::Vector2f force);
};

