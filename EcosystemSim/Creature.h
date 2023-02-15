#pragma once
#include <SFML/Graphics.hpp>
#include "global.h"
#include "Collider.h"

enum CreatureType { Prey, Predator};

class Creature
{
	public:
		CreatureType type;
		sf::Vector2f pos;
		Collider collider;

		Creature(CreatureType _type = CreatureType::Prey);
		~Creature();
		void update(float deltaTime);
		void move(sf::Vector2f move);
};

