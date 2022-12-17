#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine(float mapSize) :
	spatialGrid(SpatialGrid(mapSize))
{
	worldLimit = mapSize / 2.f;
}

sf::Vector2f PhysicsEngine::move(CircleCollider* mover, sf::Vector2f moveBy)
{
	sf::Vector2f targetPos = *mover->pPosition + moveBy;

	// Check for map bounds
	if (targetPos.x - mover->radius < -worldLimit) targetPos.x = -worldLimit + mover->radius;
	if (targetPos.x + mover->radius > worldLimit) targetPos.x = worldLimit - mover->radius;
	if (targetPos.y - mover->radius < -worldLimit) targetPos.y = -worldLimit + mover->radius;
	if (targetPos.y + mover->radius > worldLimit) targetPos.y = worldLimit - mover->radius;

	// Check for other colliders
	bool firstCollision = true;
	std::vector<CircleCollider*> colliders = spatialGrid.getColliders(*mover->pPosition, mover->radius * 2.f, moveBy);
	for (CircleCollider* collider : colliders)
	{
		// Ignore self
		if (collider == mover) continue;

		sf::Vector2f colliderPos = *collider->pPosition;

		float dx = targetPos.x - colliderPos.x;
		float dy = targetPos.y - colliderPos.y;
		float distSqr = dx*dx + dy*dy;
		// Check for collision
		float rSum = mover->radius + collider->radius;
		float intersection = distSqr - rSum*rSum;
		if (intersection < 0.f) {
			// Adjust position
			float dist = sqrt(distSqr);
			sf::Vector2f normalized(dx / dist, dy / dist);
			if (firstCollision) {
				firstCollision = false;
				targetPos = colliderPos + normalized * rSum;
			}
			else
				targetPos = (targetPos + (colliderPos + normalized * rSum)) / 2.f;
		}
	}
	spatialGrid.move(mover, targetPos);
	return targetPos;

}

void PhysicsEngine::init(CircleCollider* collider)
{
	colliders.push_back(collider);
}

void PhysicsEngine::remove(CircleCollider* collider)
{
	for (size_t i = 0; i < colliders.size(); i++)
	{
		if (colliders[i] == collider) {
			colliders.erase(colliders.begin() + i);
			break;
		}
	}
}
