#include "CameraController.h"

CameraController::CameraController(sf::View& _view):
	view(_view) 
{
	view.zoom(zoom);
	view.setCenter(sf::Vector2f(6400.f, 6400.f));
}

void CameraController::update(float deltaTime)
{
	// Moving the camera
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		int x = sf::Mouse::getPosition().x;
		int y = sf::Mouse::getPosition().y;
		int dx = lastX - x;
		int dy = lastY - y;
		lastX = x;
		lastY = y;

		if (firstPass) firstPass = false;
		else {
			position = view.getCenter() + sf::Vector2f(dx * camSpeed * zoom, dy * camSpeed * zoom);
			view.setCenter(position);
		}
	}
	else firstPass = true;

	// Set zoom
	view.zoom(zoom);
}

void CameraController::zoomEvent(float x)
{
	zoom -= (x * zoom) * zoomSpeed;
	zoom = (zoom < 1.f) ? 1.f : (zoom > 50.f) ? 50.f : zoom;
}


