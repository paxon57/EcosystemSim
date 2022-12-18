#include "SpatialGridCell.h"

SpatialGridCell::SpatialGridCell():
colliders(std::vector<CircleCollider*>()){}

void SpatialGridCell::add(CircleCollider* colliderPtr)
{
	colliders.push_back(colliderPtr);
}

void SpatialGridCell::remove(CircleCollider* colliderPtr)
{
	for (std::_Vector_iterator it = colliders.begin(); it != colliders.end(); it++)
	{
		if (*it == colliderPtr) {
			colliders.erase(it);
			break;
		}
	}
}