#include <SFML/Graphics.hpp>
#include "cube.hpp"



void Cube::draw(sf::RenderWindow& window) {
    
    A.draw(window, x0, y0);
    B.draw(window, x0, y0);
    C.draw(window, x0, y0);
    D.draw(window, x0, y0);
    E.draw(window, x0, y0);
    F.draw(window, x0, y0);
    
}