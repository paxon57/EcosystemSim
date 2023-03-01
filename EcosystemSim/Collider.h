#pragma once
#include "SFML/Graphics.hpp"

class Collider
{
	public:
		Collider(sf::Vector2f _pos = sf::Vector2f(), float _radius = 50.f);

		float radius;
		float maxSpeed = 100.f;
		float drag = 0.1;
		int gridIdx;
		int idx;

		sf::Vector2f pos;

		void addForce(sf::Vector2f force);
		void update(float dt);

	private:
		sf::Vector2f lastPos;
		sf::Vector2f velocity;
		sf::Vector2f acceleration;

		void addDrag(float dt);
};

