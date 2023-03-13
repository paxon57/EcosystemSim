#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include "global.h"
#include "PhysicsEngine.h"
#include "NEAT.h"

const float PI = 3.14159265358979323846;

struct CreatureSettings {
	int hp = 4;
	int dmg = 2;
	int rayAmount = 5;
	float rayDistance = 10;
	float fov = 90;
};

class Creature
{
	public:
		CreatureType type;

		float fov = PI/2.f;
		float rayLength = 1000.f;
		float rotation = 0.f;
		float hp = 4.f;
		int rayAmount = 5;
		int dmg = 2;
		int colliderIndex = -1;

		NEAT net;
		sf::Vector2f pos;

		Creature(PhysicsEngine& _phys, sf::Vector2f _pos, CreatureType _type = CreatureType::Prey);
		void update(float deltaTime);
		void draw();
		void applySettings(CreatureSettings settings);

		Creature& operator= (const Creature& x) {
			if (&x == this) return *this;
			type = x.type;
			fov = x.fov;
			rayLength = x.rayLength;
			rotation = x.rotation;
			hp = x.hp;
			rayAmount = x.rayAmount;
			dmg = x.dmg;
			colliderIndex = x.colliderIndex;
			net = x.net;
			pos = x.pos;
			rayAngDiff = x.rayAngDiff;
			phys = x.phys;
			ray = x.ray;
			return *this;
		}

	private:
		float rayAngDiff;

		std::vector<Raycast> ray;

		PhysicsEngine* phys;

		void forwardForce(float force);
};

