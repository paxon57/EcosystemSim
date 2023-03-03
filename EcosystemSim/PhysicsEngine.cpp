#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine(float _size) :
	size(_size),
	colliders()
{
}

int PhysicsEngine::newCollider(sf::Vector2f _pos, float _radius)
{
	int index = -1;
	// Look for free spot in array
	for (int i = 0; i < 20000; i++)
	{
		// Check if free
		if (colliders[i].idx == -1) {
			index = i;

			// Add collider
			colliders[i] = Collider(_pos, _radius);
			colliders[i].idx = index;

			// Add imperfectinos
			colliders[i].radius += (rand() / (float)RAND_MAX) - 0.5f;

			int gridIndex = getGridIndexFromPos(_pos);
			colliders[i].gridIdx = gridIndex;

			// Add index to grid
			grid[gridIndex].colliderIdx.push_back(index);

			return index;
		}
	}
	return -1;
}

void PhysicsEngine::removeCollider(int index)
{
	// Remove from grid
	int gridIndex = colliders[index].gridIdx;
	grid[gridIndex].remove(index);
	// Disable from main array
	colliders[index].idx = -1;
}

void PhysicsEngine::update(float dt, int subSteps)
{
	// Update all colliders
	for (int i = 0; i < 20000; i++)
	{
		if (colliders[i].idx == -1) break;

		colliders[i].update(dt);
		keepInBounds(i);
		recalculateIndex(i);
	}

	// Collision handling
	float subDt = dt / subSteps;
	for (int i = 0; i < subSteps; i++)
	{
		physUpdate(subDt);
	}
}

sf::Vector2f PhysicsEngine::getPos(int index)
{
	return colliders[index].pos;
}

void PhysicsEngine::move(int index, sf::Vector2f newPos)
{
	colliders[index].pos = newPos;
	keepInBounds(index);
	recalculateIndex(index);
}

void PhysicsEngine::addForce(int colliderIndex, sf::Vector2f force)
{
	colliders[colliderIndex].addForce(force);
}

void PhysicsEngine::raycast(int senderIndex, Raycast& ray, sf::Vector2f start, sf::Vector2f end)
{
	// Reset ray
	ray.targetIndex = -1;
	ray.hit = false;
	ray.hitWorld = false;
	// Vector of colliders
	int found[100];
	int foundIndex = 0;
	// Get boundaries
	float minX, minY, maxX, maxY;
	minX = (start.x < end.x) ? start.x : end.x;
	maxX = (start.x > end.x) ? start.x : end.x;
	minY = (start.y < end.y) ? start.y : end.y;
	maxY = (start.y > end.y) ? start.y : end.y;

	// Line equation: y = mx + b
	// Slope
	float m = (end.y - start.y) / (end.x - start.x);
	// B offset
	float b = start.y - (start.x * m);

	// Find intersections with grid along X
	for (int i = floor(minX / 100.f); i <= ceil(maxX / 100.f); i++)
	{
		// Calculate y value at the grid edge
		float y = m * (i * 100.f) + b;
		// Add left/right cell contents
		int index = getGridIndexFromPos(sf::Vector2f(i * 100.f - 1.f, y));
		if (indexIsValid(index)) for (int idx : grid[index].colliderIdx) {
			found[foundIndex] = idx;
			foundIndex++;
		}
		index = getGridIndexFromPos(sf::Vector2f(i * 100.f + 1.f, y));
		if (indexIsValid(index)) for (int idx : grid[index].colliderIdx) {
			found[foundIndex] = idx;
			foundIndex++;
		}
	}

	// Find intersections with grid along Y
	for (int i = floor(minY / 100.f); i <= ceil(maxY / 100.f); i++)
	{
		// Calculate x value at the grid edge
		float x = ((i * 100.f) - b)/m;
		// Add left/right cell contents
		int index = getGridIndexFromPos(sf::Vector2f(x, i * 100.f - 1.f));
		if (indexIsValid(index)) for (int idx : grid[index].colliderIdx) {
			found[foundIndex] = idx;
			foundIndex++;
		}
		index = getGridIndexFromPos(sf::Vector2f(x, i * 100.f + 1.f));
		if (indexIsValid(index)) for (int idx : grid[index].colliderIdx) {
			found[foundIndex] = idx;
			foundIndex++;
		}
	}

	// Find closest intersection
	float closestDistSqr = 9999999999999.f;
	for (int i = 0; i < foundIndex; i++) {
		int idx = found[i];

		// Prevent self detection
		if (idx == senderIndex) continue;

		// Check for distance from line to the collider centre
		float d = abs(m * colliders[idx].pos.x - colliders[idx].pos.y + b) / sqrt(m * m + 1);
		// Check for intersection
		if (d < colliders[idx].radius) {
			sf::Vector2f diff = colliders[idx].pos - start;
			float distSqr = diff.x * diff.x + diff.y * diff.y;
			if (distSqr < closestDistSqr) {
				closestDistSqr = distSqr;
				ray.hit = true;
				ray.targetIndex = idx;
				ray.distance = sqrt(distSqr);
			}
		}
	}

	// Check for world hit if not colliders were hit
	if (!ray.hit) {
		sf::Vector2f hitPos = end;
		if (minX < 0.f) {
			ray.hitWorld = true;
			hitPos.x = 0.f;
			hitPos.y = b;
			sf::Vector2f diff = hitPos - start;
			ray.distance = sqrt(diff.x * diff.x + diff.y * diff.y);
		}
		else if (maxX > 12800.f) {
			ray.hitWorld = true;
			hitPos.x = 12800.f;
			hitPos.y = m * 12800.f + b;
			sf::Vector2f diff = hitPos - start;
			ray.distance = sqrt(diff.x * diff.x + diff.y * diff.y);
		}
		else if (minY < 0.f) {
			ray.hitWorld = true;
			hitPos.y = 0.f;
			hitPos.x = (-b) / m;
			sf::Vector2f diff = hitPos - start;
			ray.distance = sqrt(diff.x * diff.x + diff.y * diff.y);
		}
		else if (maxY > 12800.f) {
			ray.hitWorld = true;
			hitPos.y = 12800.f;
			hitPos.x = (12800.f - b) / m;
			sf::Vector2f diff = hitPos - start;
			ray.distance = sqrt(diff.x * diff.x + diff.y * diff.y);
		}
	}
	// Set distance if nothing hit
	if (!ray.hit && !ray.hitWorld) {
		sf::Vector2f diff = end - start;
		ray.distance = sqrt(diff.x * diff.x + diff.y * diff.y);
	}
}

int PhysicsEngine::getGridIndexFromPos(sf::Vector2f _pos)
{
	int x = fmod((int)(_pos.x / 100.f), 128);
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
	//physLoop(0, 127, dt);
	// Threading
	int dx = floor(128 / numThreads);

	for (int i = 0; i < numThreads; i++)
	{
		int x1 = dx * i;
		int x2;
		if (i == numThreads - 1) x2 = 128;
		else x2 = dx * (i + 1);
		futures.push_back(std::async([this, x1, x2, dt] { physLoop(x1, x2, dt); }));
	}

	futures.clear();
}

void PhysicsEngine::physLoop(int x1, int x2, float dt)
{
	deltaTime = dt;

	// Loop through grid
	for (int x = x1; x < x2; x++)
	{
		for (int y = 0; y < 128; y++)
		{
			int cellIndex = getGridIndex(x, y);
			// Loop through neighbours
			for (int dx = -1; dx <= 1; dx++)
			{
				if (x + dx < 0 || x + dx >= 128) continue;
				for (int dy = -1; dy <= 1; dy++)
				{
					if (y + dy < 0 || y + dy >= 128) continue;
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
				solveCollision(index1, index2);
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

	//recalculateIndex(_index);
	//recalculateIndex(_otherIndex);
}

void PhysicsEngine::keepInBounds(int _index)
{
	if (colliders[_index].pos.x < colliders[_index].radius) colliders[_index].pos.x = colliders[_index].radius;
	if (colliders[_index].pos.y < colliders[_index].radius) colliders[_index].pos.y = colliders[_index].radius;
	if (colliders[_index].pos.x > size - colliders[_index].radius) colliders[_index].pos.x = size - colliders[_index].radius;
	if (colliders[_index].pos.y > size - colliders[_index].radius) colliders[_index].pos.y = size - colliders[_index].radius;
}

void PhysicsEngine::recalculateIndex(int _index)
{
	int newGridIndex = getGridIndexFromPos(colliders[_index].pos);
	if (newGridIndex != colliders[_index].gridIdx) {
		grid[colliders[_index].gridIdx].remove(_index);
		grid[newGridIndex].colliderIdx.push_back(_index);
		colliders[_index].gridIdx = newGridIndex;
	}
}

bool PhysicsEngine::indexIsValid(int _index)
{
	return (_index > 0 && _index < 128*128);
}
