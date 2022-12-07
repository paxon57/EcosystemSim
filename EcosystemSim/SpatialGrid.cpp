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

std::vector<CircleCollider*> SpatialGrid::getColliders(sf::Vector2f pos, float radius)
{
	std::vector<CircleCollider*> colliders;
	int i = getIndex(pos.x, pos.y);
	if (i == -1) return colliders;

	int mainI = i;
	for (CircleCollider* pCollider : grid[i].colliders) {
		colliders.push_back(pCollider);
	}
	// TODO? Check corners
	// Check left
	i = getIndex(pos.x - radius, pos.y);
	if (i != mainI && i != -1) {
		for (CircleCollider* pCollider : grid[i].colliders) {
			colliders.push_back(pCollider);
		}
	}
	// Check right
	i = getIndex(pos.x + radius, pos.y);
	if (i != mainI && i != -1) {
		for (CircleCollider* pCollider : grid[i].colliders) {
			colliders.push_back(pCollider);
		}
	}
	// Check top
	i = getIndex(pos.x, pos.y + radius);
	if (i != mainI && i != -1) {
		for (CircleCollider* pCollider : grid[i].colliders) {
			colliders.push_back(pCollider);
		}
	}
	// Check down
	i = getIndex(pos.x, pos.y - radius);
	if (i != mainI && i != -1) {
		for (CircleCollider* pCollider : grid[i].colliders) {
			colliders.push_back(pCollider);
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
