#include "SpatialGridCell.h"
#include "CircleCollider.h"

CircleCollider::CircleCollider(sf::Vector2f* _position, float _radius)
{
	radius = _radius;
	pPosition = _position;
}
