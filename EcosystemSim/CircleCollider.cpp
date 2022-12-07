#include "CircleCollider.h"

CircleCollider::CircleCollider(sf::Vector2f* _position, float _radius):
	spatialGridCell(nullptr)
{
	radius = _radius;
	pPosition = _position;
}
