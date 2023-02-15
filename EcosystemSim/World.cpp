#include "World.h"

World::World(float _size){
	size = _size;

	area = sf::RectangleShape(sf::Vector2f(size, size));
	area.setFillColor(sf::Color(236, 240, 241));
	area.setOutlineColor(sf::Color(52, 73, 94));
	area.setOutlineThickness(300.f);
}

void World::draw()
{
	window.draw(area);
}

