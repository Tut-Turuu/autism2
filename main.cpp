#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "cube.hpp"
#include <random>
#include <time.h>

int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(800, 700), "Spining Cube", sf::Style::Default, settings);
    Cube cube(400, 350, {-100, 0, 0}, {0, -100, 0}, {rand()%256, rand()%256, rand()%256});

    bool mouse_pressed = false;

    double last_x;
    double last_y;
    int i = 0;


    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    mouse_pressed = true;
                    last_x = sf::Mouse::getPosition().x;
                    last_y = sf::Mouse::getPosition().y;
                    break;
                case sf::Event::MouseButtonReleased:
                    mouse_pressed = false;
                    break;
            }
            
        }

        if (mouse_pressed) {
            // if (i == 5) {
            //     window.close();
            // }
            // ++i;
            cube.move(Vector2d(sf::Mouse::getPosition().x - last_x, sf::Mouse::getPosition().y - last_y));

            last_x = sf::Mouse::getPosition().x;
            last_y = sf::Mouse::getPosition().y;
        }

        cube.draw(window);

        window.display();
        window.clear();

    }
    
}