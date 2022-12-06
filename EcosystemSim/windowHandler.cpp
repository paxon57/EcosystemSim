#include <iostream>
#include <SFML/Graphics.hpp>

void windowHandler(sf::RenderWindow& window) {
    sf::Event evnt;
    while (window.pollEvent(evnt)) {
        switch (evnt.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized:
            printf("New window width: %i New window height: %i\n", evnt.size.width, evnt.size.height);
            break;
        }
    }
}