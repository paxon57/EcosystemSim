#pragma once
#include "SFML/Graphics.hpp"

enum CreatureType { Prey, Predator };

class Collider
{
	public:
		Collider(sf::Vector2f _pos = sf::Vector2f(), float _radius = 50.f, CreatureType _type = CreatureType::Prey);

		float radius;
		float maxSpeed = 100.f;
		float drag = 0.1;
		int gridIdx;
		int idx;

		sf::Vector2f pos;

		CreatureType type;

		void addForce(sf::Vector2f force);
		void update(float dt);

	private:
		sf::Vector2f lastPos;
		sf::Vector2f velocity;
		sf::Vector2f acceleration;

		void addDrag(float dt);
};

