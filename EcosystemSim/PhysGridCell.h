#pragma once
#include <vector>
#include "Collider.h"

class PhysGridCell
{
public:
	std::vector<Collider*> colliders;
};

