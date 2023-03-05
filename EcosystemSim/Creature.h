#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include "global.h"
#include "Collider.h"
#include "PhysicsEngine.h"

const float PI = 3.14159265358979323846;

class Creature
{
	public:
		CreatureType type;

		float fov = PI/2.f;
		float rayLength = 1000.f;
		float rotation = 0.f;
		int rayAmount = 5;
		int colliderIndex;

		Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type = CreatureType::Prey);
		void update(float deltaTime);
		void draw();

	private:
		float rayAngDiff;

		sf::Vector2f pos;
		PhysicsEngine& phys;
		
		std::vector<Raycast> ray;
};

