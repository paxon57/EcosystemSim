#pragma once
#include <SFML/Graphics.hpp>

class Raycast
{
	public:
		Raycast();

		bool hit = false;
		bool hitWorld = false;
		int targetIndex = -1;
		float distance = 0.f;
	private:
		
};

