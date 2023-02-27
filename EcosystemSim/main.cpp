#include <SFML/Graphics.hpp>
#include <iostream>
#include "global.h"
#include "World.h"
#include "CameraController.h"
#include "Creature.h"
#include "PhysicsEngine.h"

sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(1024, 1024), "Ecosystem Simulation", sf::Style::Close | sf::Style::Resize);
sf::View view(sf::Vector2f(50.f, 50.f), sf::Vector2f(window.getSize()));
sf::Clock frameTimeClock;

World world(12800.f);
PhysicsEngine phys(12800.f);
CameraController cam(view);

sf::VertexArray creaturesQuads(sf::Quads, 4 * 20000);
sf::Texture creatureTexture;

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

int main()
{
    // Load textures
    if (!creatureTexture.loadFromFile("creature.png")) return 0;

    // Random seed
    srand(time(NULL));

    // Test //////////////////////////////////
    const int numCreatures = 15000;
    int creaturesLeft = numCreatures;
    float interval = 0.5f;
    float curInterval = interval;
    std::vector<Creature> creatures;
    /////////////////////////////////////////
    
    unsigned long frame = 0;

    // Main loop
    while (window.isOpen()) {
        // Window setup
        eventHandler(window);
        window.clear(sf::Color(127, 140, 141));
        window.setView(view);
        // TODO??? Clear creaturesQuad
        
        view.setSize(sf::Vector2f(window.getSize()));
        
        // Get frame time
        frame += 1;
        float deltaTime = frameTimeClock.getElapsedTime().asSeconds();
        frameTimeClock.restart();
        printf("Frame: %lu | Frametime: %fms | FPS: %f\n", frame, deltaTime*1000.f, 1.f/deltaTime);

        // Updates
        cam.update(deltaTime);
        world.draw();
        // Test ///////////////////////////////
        curInterval -= deltaTime;
        int toSpawn = 0.f;
        if (curInterval < 0.f) {
            toSpawn = 100;
            curInterval = interval;
        }
        if (toSpawn > creaturesLeft) toSpawn = creaturesLeft;
        creaturesLeft -= toSpawn;

        for (size_t i = 0; i < toSpawn; i++)
        {
            float x = 1400.f + i * 100.f;
            //float y = 1400.f + (rand() / (float)RAND_MAX) * 5000.f;
            float y = 1000.f;
            bool type = round(rand() / (float)RAND_MAX);
            if (type) creatures.emplace_back(Creature(phys, sf::Vector2f(x, y), CreatureType::Predator));
            else creatures.emplace_back(Creature(phys, sf::Vector2f(x, y), CreatureType::Prey));
        }

        for (Creature creature : creatures) {
            creature.update(deltaTime);
        }

        phys.update(deltaTime, 1);

        for (Creature creature : creatures) {
            creature.draw();
        }
        ////////////////////////////////////
        window.draw(creaturesQuads, &creatureTexture);
        window.display();
    }

    return 0;
}
