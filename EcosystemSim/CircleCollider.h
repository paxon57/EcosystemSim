#pragma once
#include <SFML/Graphics.hpp>

class SpatialGridCell;

class CircleCollider
{
	public:
		CircleCollider(sf::Vector2f* _position = nullptr, float _radius = 50.f);
		sf::Vector2f* pPosition;
		SpatialGridCell* spatialGridCell;
		float radius;
};

