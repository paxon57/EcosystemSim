#include "SpatialGrid.h"

SpatialGrid::SpatialGrid(float worldSize) :
cellSize(worldSize / 10.f) {}

void SpatialGrid::move(CircleCollider* pCollider, sf::Vector2f pos)
{
	int x = floor(pos.x / cellSize);
	int y = floor(pos.y / cellSize);
	int i = y * 10 + x;

	SpatialGridCell targetCell = grid[i];
	SpatialGridCell* ptr = pCollider->spatialGridCell;
	if (ptr == nullptr) {
		grid[i].add(pCollider);
		pCollider->spatialGridCell = &grid[i];
		return;
	}
	else if (ptr != &targetCell) {
		ptr->remove(pCollider);
		targetCell.add(pCollider);
		pCollider->spatialGridCell = &grid[i];
	}
}

std::vector<CircleCollider*> SpatialGrid::getColliders(sf::Vector2f pos, float radius)
{
	std::vector<CircleCollider*> colliders;
	int i = getIndex(pos.x, pos.y);
	int mainI = i;
	std::vector<CircleCollider*> cellColliders = grid[i].getColliders();
	colliders.insert(colliders.end(), cellColliders.begin(), cellColliders.end());
	// TODO? Check corners
	// Check left
	i = getIndex(pos.x - radius, pos.y);
	if (i != mainI && (pos.x - radius >= -phys.worldLimit)) {
		std::vector<CircleCollider*> cellColliders = grid[i].getColliders();
		colliders.insert(colliders.end(), cellColliders.begin(), cellColliders.end());
	}
	// Check right
	i = getIndex(pos.x + radius, pos.y);
	if (i != mainI && (pos.x + radius <= phys.worldLimit)) {
		std::vector<CircleCollider*> cellColliders = grid[i].getColliders();
		colliders.insert(colliders.end(), cellColliders.begin(), cellColliders.end());
	}
	// Check top
	i = getIndex(pos.x, pos.y + radius);
	if (i != mainI && (pos.y + radius <= phys.worldLimit)) {
		std::vector<CircleCollider*> cellColliders = grid[i].getColliders();
		colliders.insert(colliders.end(), cellColliders.begin(), cellColliders.end());
	}
	// Check down
	i = getIndex(pos.x, pos.y - radius);
	if (i != mainI && (pos.y - radius >= -phys.worldLimit)) {
		std::vector<CircleCollider*> cellColliders = grid[i].getColliders();
		colliders.insert(colliders.end(), cellColliders.begin(), cellColliders.end());
	}
	return colliders;
}

int SpatialGrid::getIndex(float x, float y)
{
	x = floor(x / cellSize);
	y = floor(y / cellSize);
	return y * 10 + x;
}
