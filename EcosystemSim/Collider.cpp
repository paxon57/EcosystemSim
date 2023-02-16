#include "Collider.h"

Collider::Collider(sf::Vector2f _pos, float _radius) :
	radius(_radius),
	gridIdx(0),
	pos(_pos),
	idx(-1) {}
