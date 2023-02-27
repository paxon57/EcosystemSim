#pragma once
#include <SFML/Graphics.hpp>

class CameraController
{
	
	public:
		CameraController(sf::View& _view);
		void update(float deltaTime);
		void zoomEvent(float x);

	private:
		float camSpeed = 1.f;
		float zoomSpeed = .25f;

		int lastX = 0;
		int lastY = 0;
		bool firstPass = true;
		float zoom = 20.f;

		sf::View& view;
		sf::Vector2f position = sf::Vector2f(0.f, 0.f);
};

