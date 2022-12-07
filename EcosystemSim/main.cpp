#include <SFML/Graphics.hpp>
#include <iostream>
#include "global.h"
#include "World.h"
#include "CameraController.h"
#include "Creature.h"

sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(512, 512), "Ecosystem Simulation", sf::Style::Close | sf::Style::Resize);
sf::View view(sf::Vector2f(50.f, 50.f), sf::Vector2f(window.getSize()));
sf::CircleShape preyBody = sf::CircleShape(50.f);
sf::CircleShape predatorBody = sf::CircleShape(50.f);
sf::Clock frameTimeClock;

World world = World(12800.f);
CameraController cam(view);
PhysicsEngine phys = PhysicsEngine(12800.f);

void eventHandler(sf::RenderWindow& window) {
    sf::Event evnt;
    while (window.pollEvent(evnt)) {
        switch (evnt.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized:
            printf("New window width: %i New window height: %i\n", evnt.size.width, evnt.size.height);
            break;
        case sf::Event::MouseWheelScrolled:
            cam.zoomEvent(evnt.mouseWheelScroll.delta);
            break;
        }
    }
}

void globalGraphicsInit() {
    preyBody.setFillColor(sf::Color(46, 204, 113));
    preyBody.setOutlineColor(sf::Color(44, 62, 80));
    preyBody.setOutlineThickness(-5.f);
    preyBody.setOrigin(sf::Vector2f(50.f, 50.f));

    predatorBody.setFillColor(sf::Color(231, 76, 60));
    predatorBody.setOutlineColor(sf::Color(44, 62, 80));
    predatorBody.setOutlineThickness(-5.f);
    predatorBody.setOrigin(sf::Vector2f(50.f, 50.f));
}

int main()
{
    // Init graphics for creatures
    globalGraphicsInit();
    
    // Random seed
    srand(time(NULL));

    // Test
    const int numCreatures = 10000;
    Creature creatures[numCreatures];
    for (size_t i = 0; i < numCreatures; i++)
    {
        float x = rand() / (float)RAND_MAX * 10000.f - 5000.f;
        float y = rand() / (float)RAND_MAX * 10000.f - 5000.f;
        creatures[i].pos = sf::Vector2f(x, y);
        if (rand()%2)
            creatures[i].type = CreatureType::Prey;
        else
            creatures[i].type = CreatureType::Predator;
    }
    /////////////////////////
    
    unsigned long frame = 0;

    // Main loop
    while (window.isOpen()) {
        // Window setup
        eventHandler(window);
        window.clear(sf::Color(127, 140, 141));
        window.setView(view);
        
        view.setSize(sf::Vector2f(window.getSize()));
        
        // Get frame time
        frame += 1;
        float deltaTime = frameTimeClock.getElapsedTime().asSeconds();
        frameTimeClock.restart();
        printf("Frame: %lu | Frametime: %fms | FPS: %f\n", frame, deltaTime*1000.f, 1.f/deltaTime);

        // Updates
        cam.update(deltaTime);
        world.draw();

        // Test
        for (size_t i = 0; i < numCreatures; i++)
        {
            creatures[i].update(deltaTime);
        }
        /// /////////////////////////////////


        window.display();
    }

    return 0;
}