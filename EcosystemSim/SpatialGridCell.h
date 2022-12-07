#pragma once
#include <vector>
#include "CircleCollider.h"

class SpatialGridCell
{
	public:
		SpatialGridCell();
		std::vector<CircleCollider*> colliders;

		void add(CircleCollider* colliderPtr);
		void remove(CircleCollider* colliderPtr);
};

