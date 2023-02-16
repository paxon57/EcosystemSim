#pragma once
#include "SFML/Graphics.hpp"

class Collider
{
	public:
		Collider(sf::Vector2f _pos = sf::Vector2f(), float _radius = 50.f);

		float radius;
		sf::Vector2f pos;
		int gridIdx;
		int idx;
};

