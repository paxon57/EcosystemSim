#include "PhysGridCell.h"

PhysGridCell::PhysGridCell()
{
}

void PhysGridCell::remove(int index)
{
	colliderIdx.erase(
		std::remove(colliderIdx.begin(), colliderIdx.end(), index),
		colliderIdx.end()
	);
	//std::vector<int>::iterator it = std::find();
}
