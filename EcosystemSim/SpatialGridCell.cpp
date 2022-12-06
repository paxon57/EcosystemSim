#include "CircleCollider.h"
#include "SpatialGridCell.h"

SpatialGridCell::SpatialGridCell()
{}

void SpatialGridCell::add(CircleCollider* colliderPtr)
{
	colliders.push_back(colliderPtr);
}

void SpatialGridCell::remove(CircleCollider* colliderPtr)
{
	for (size_t i = 0; i < colliders.size(); i++)
	{
		if (colliders[i] == colliderPtr) {
			colliders.erase(colliders.begin() + i);
			break;
		}
	}
}

std::vector<CircleCollider*> SpatialGridCell::getColliders()
{
	return colliders;
}
