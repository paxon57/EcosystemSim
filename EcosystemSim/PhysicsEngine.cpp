#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine(float _size):
	size(_size)
{
	// Init grid array
	for (int i = 0; i < 128*128; i++)
	{
		grid[i] = PhysGridCell();
	}
}

void PhysicsEngine::update(float dt, int subSteps)
{
	float subDt = dt / subSteps;

	for (int i = 0; i < subSteps; i++)
	{
		physUpdate(subDt);
	}
}

void PhysicsEngine::addCollider(Collider* pCollider)
{
	colliders.push_back(pCollider);
	int idx = getCellIndex(pCollider->pos.x, pCollider->pos.y);
	pCollider->gridIdx = idx;
	grid[idx].colliders.push_back(pCollider);
}

void PhysicsEngine::removeCollider(Collider* pCollider)
{
	remove(grid[pCollider->gridIdx].colliders.begin(),
		grid[pCollider->gridIdx].colliders.end(),
		pCollider);

	remove(colliders.begin(), colliders.end(), pCollider);
}

void PhysicsEngine::physUpdate(float dt)
{
	// Update every collider
	for (Collider* collider : colliders)
	{
		// Recalculate gridIdx
		recalculateCellIndex(collider);

		int i = collider->gridIdx;
		int x = i % 128;
		int y = floor(i / 128);

		// Loop around the cell
		for (int dx = -1; dx <= 1; dx++)
		{
			// Check for map edge
			if (x + dx < 0 || x + dx >= 128) continue;
			for (int dy = -1; dy < 1; dy++)
			{
				// Check for map edge
				if (y + dy < 0 || y + dy >= 128) continue;

				// Calculate current index
				int curI = i + dx + (dy * 128);

				// Collide
				for (Collider* targetCollider : grid[curI].colliders) {
					if (targetCollider == nullptr) continue;
					// Check for self
					if (targetCollider == collider) continue;

					// Check for collision
					sf::Vector2f diff = collider->pos - targetCollider->pos;
					float dist = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
					float intersection = collider->radius + targetCollider->radius - dist;
					if (intersection > 0) {
						// Separate colliders
						sf::Vector2f normalizedDiff = diff / dist;
						collider->pos += normalizedDiff * (intersection / 2);
						targetCollider->pos -= normalizedDiff * (intersection / 2);
						keepInBounds(targetCollider);
						// Recalculate grid index
						//recalculateCellIndex(targetCollider); ?????
					}
				}
			}
		}
		keepInBounds(collider);

		
	}
}

int PhysicsEngine::getCellIndex(float _x, float _y)
{
	int x = floorf(_x / 100.f);
	int y = floorf(_y / 100.f);
	int idx = y * 128 + x;
	return idx;
}

void PhysicsEngine::recalculateCellIndex(Collider* pCollider)
{
	int targetIndex = getCellIndex(pCollider->pos.x, pCollider->pos.y);

	int currentIndex = pCollider->gridIdx;
	if (currentIndex != targetIndex) {
		remove(grid[currentIndex].colliders.begin(), grid[currentIndex].colliders.end(), pCollider);
		grid[targetIndex].colliders.push_back(pCollider);
		pCollider->gridIdx = targetIndex;
	}
}

void PhysicsEngine::keepInBounds(Collider* pCollider) {
	if (pCollider->pos.x < pCollider->radius) pCollider->pos.x = pCollider->radius;
	if (pCollider->pos.y < pCollider->radius) pCollider->pos.y = pCollider->radius;
	if (pCollider->pos.x + pCollider->radius > 12800.f) pCollider->pos.x = 12800.f - pCollider->radius;
	if (pCollider->pos.y + pCollider->radius > 12800.f) pCollider->pos.y = 12800.f - pCollider->radius;
}
