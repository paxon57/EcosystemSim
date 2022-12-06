#pragma once
#include <SFML/Graphics.hpp>
#include "CircleCollider.h"
#include "global.h"

enum CreatureType { Prey, Predator};

class Creature
{
	public:
		CreatureType type;
		sf::Vector2f pos;

		Creature(CreatureType _type = CreatureType::Prey);
		~Creature();
		void update(float deltaTime);

	private:
		CircleCollider collider;
		
};

