#pragma once
#include <vector>

class CircleCollider;

class SpatialGridCell
{
	public:
		SpatialGridCell();
		std::vector<CircleCollider*> colliders;

		void add(CircleCollider* colliderPtr);
		void remove(CircleCollider* colliderPtr);
		std::vector<CircleCollider*> getColliders();
};

