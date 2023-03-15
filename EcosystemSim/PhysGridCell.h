#pragma once
#include <vector>
#include <algorithm>
#include "Collider.h"

class PhysGridCell
{
	public:
		PhysGridCell();

		std::vector<int> colliderIdx;

		void remove(int index);
};

