#pragma once
#include <SFML/Graphics.hpp>
#include "global.h"

class World
{
	public: 
		World(float _size);
		void draw();

	private:
		float size;

		sf::RectangleShape area;
		
};

