#include "SpatialGrid.h"
#include "global.h"

SpatialGrid::SpatialGrid(float worldSize) :
cellSize(worldSize / GRID_SIZE) {}

void SpatialGrid::move(CircleCollider* pCollider, sf::Vector2f pos)
{
	int i = getIndex(pos.x, pos.y);
	if (i == -1) return;

	SpatialGridCell* targetCell = &grid[i];
	SpatialGridCell* ptr = pCollider->spatialGridCell;
	if (ptr == nullptr) {
		grid[i].add(pCollider);
		pCollider->spatialGridCell = &grid[i];
	}
	else if (ptr != targetCell) {
		ptr->remove(pCollider);
		targetCell->add(pCollider);
		pCollider->spatialGridCell = targetCell;
	}
}

std::vector<CircleCollider*> SpatialGrid::getColliders(sf::Vector2f pos, float distance, sf::Vector2f direction)
{
	std::vector<CircleCollider*> colliders;
	int mainI = getIndex(pos.x, pos.y);
	if (mainI == -1) return colliders;

	int cellsInRange = ceil(distance / cellSize);
	int xP = (direction.x > 0.f) ? cellsInRange : 0;
	int xN = (direction.x < 0.f) ? cellsInRange : 0;
	int yP = (direction.y > 0.f) ? cellsInRange : 0;
	int yN = (direction.y < 0.f) ? cellsInRange : 0;

	for (size_t dx = -xN; dx <= xP; dx++) 
	{
		int x = (mainI % GRID_SIZE) + dx;
		if (x < 0 || x > GRID_SIZE) continue;

		for (size_t dy = -yN; dy < yP; dy++)
		{
			int y = (floor(mainI / GRID_SIZE)) + dy;
			if (y < 0 || y > GRID_SIZE) continue;

			int index = y * GRID_SIZE + x;
			for (CircleCollider* pCollider : grid[index].colliders) {
				colliders.push_back(pCollider);
			}
		}
	}

	return colliders;
}

int SpatialGrid::getIndex(float x, float y)
{
	x += phys.worldLimit;
	y += phys.worldLimit;
	x = floor(x / cellSize);
	y = floor(y / cellSize);

	if (x < 0)				return -1;
	if (x > GRID_SIZE - 1)	return -1;
	if (y < 0)				return -1;
	if (y > GRID_SIZE - 1)	return -1;

	return y * GRID_SIZE + x;
}
