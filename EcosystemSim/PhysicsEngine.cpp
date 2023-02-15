#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine(float _size) :
	size(_size)
{
}

int PhysicsEngine::newCollider(sf::Vector2f _pos, float _radius)
{
	int index = -1;
	// Look for free spot in array
	for (int i = 0; i < 20000; i++)
	{
		// Check if free
		if (colliders[i].index == -1) {
			index = i;

			// Add collider
			colliders[i] = Collider(_pos, _radius);
			colliders[i].index = i;
			int gridIndex = getGridIndexFromPos(_pos);
			colliders[i].gridIdx = gridIndex;

			// Add index to grid
			grid[gridIndex].colliderIdx.push_back(i);

			break;
		}
	}

	return index;
}

void PhysicsEngine::removeCollider(int index)
{
	// Remove from grid
	int gridIndex = colliders[index].gridIdx;
	grid[gridIndex].remove(index);
	// Disable from main array
	colliders[index].index = -1;
}

void PhysicsEngine::update(float dt, int subSteps)
{
	float subDt = dt / subSteps;
	for (int i = 0; i < subSteps; i++)
	{
		physUpdate(subDt);
	}
}

sf::Vector2f* PhysicsEngine::getPosPointer(int index)
{
	return &colliders[index].pos;
}

void PhysicsEngine::move(int index, sf::Vector2f newPos)
{
	colliders[index].pos = newPos;
	keepInBounds(index);
	int newGridIndex = getGridIndexFromPos(colliders[index].pos);
	if (newGridIndex != colliders[index].gridIdx) {
		grid[colliders[index].gridIdx].remove(index);
		grid[newGridIndex].colliderIdx.push_back(index);
		colliders[index].gridIdx = newGridIndex;
	}
}

int PhysicsEngine::getGridIndexFromPos(sf::Vector2f _pos)
{
	int x = floorf((int)(_pos.x / 100.f) % 128);
	int y = floorf(_pos.y / 100.f);
	int i = y * 128 + x;
	return i;
}

int PhysicsEngine::getGridIndex(int _x, int _y)
{
	return _y * 128 + _x;
}

void PhysicsEngine::physUpdate(float dt)
{
	// Loop through grid without edges
	for (int x = 1; x < 127; x++)
	{
		for (int y = 1; y < 127; y++)
		{
			int cellIndex = getGridIndex(x, y);
			// Loop through neighbours
			for (int dx = -1; dx <= 1; dx++)
			{
				for (int dy = -1; dy <= 1; dy++)
				{
					int otherCellIndex = getGridIndex(x + dx, y + dy);
					checkCellCollisions(cellIndex, otherCellIndex);
				}
			}
		}
	}
}

void PhysicsEngine::checkCellCollisions(int _cellIndex, int _otherCellIndex)
{
	for (int index1 : grid[_cellIndex].colliderIdx) {
		for (int index2 : grid[_otherCellIndex].colliderIdx) {
			// Skip if same object
			if (index1 == index2) continue;
			// Check collision
			if (collide(index1, index2)) {
				
			}
		}
	}
}

bool PhysicsEngine::collide(int _index, int _otherIndex)
{
	float sum = colliders[_index].radius + colliders[_otherIndex].radius;
	sf::Vector2f diff = colliders[_index].pos - colliders[_otherIndex].pos;
	float distSqr = diff.x*diff.x + diff.y*diff.y;
	return (distSqr < sum * sum);
}

void PhysicsEngine::solveCollision(int _index, int _otherIndex)
{
	float sum = colliders[_index].radius + colliders[_otherIndex].radius;
	sf::Vector2f diff = colliders[_index].pos - colliders[_otherIndex].pos;
	float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
	sf::Vector2f normalized = sf::Vector2f(diff.x / dist, diff.y / dist);

	float intersection = sum - dist;
	colliders[_index].pos += normalized * (intersection / 2.f);
	colliders[_otherIndex].pos -= normalized * (intersection / 2.f);

	keepInBounds(_index);
	keepInBounds(_otherIndex);
}

void PhysicsEngine::keepInBounds(int _index)
{
	if (colliders[_index].pos.x < colliders[_index].radius) colliders[_index].pos.x = colliders[_index].radius;
	if (colliders[_index].pos.y < colliders[_index].radius) colliders[_index].pos.y = colliders[_index].radius;
	if (colliders[_index].pos.x > size - colliders[_index].radius) colliders[_index].pos.x = size - colliders[_index].radius;
	if (colliders[_index].pos.y > size - colliders[_index].radius) colliders[_index].pos.y = size - colliders[_index].radius;
}
