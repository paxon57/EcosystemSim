#include "PhysGridCell.h"

PhysGridCell::PhysGridCell()
{
}

void PhysGridCell::remove(int index)
{
	/*colliderIdx.erase(
		std::remove(colliderIdx.begin(), colliderIdx.end(), index),
		colliderIdx.end()
	);*/
	for (int i = 0; i < colliderIdx.size(); i++)
	{
		if (colliderIdx[i] == index) {
			colliderIdx.erase(colliderIdx.begin() + i);
			return;
		}
	}
}
