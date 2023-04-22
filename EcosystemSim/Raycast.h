#pragma once
#include <SFML/Graphics.hpp>

class Raycast
{
	public:
		Raycast();

		bool hit = false;
		int targetIndex = -1;
		float distance = 0.f;
		bool prey = false;
	private:
		
};

