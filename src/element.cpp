#include "../include/element.h"


Element::Element() 
{ 
    vel = sf::Vector2f(0.0f, 0.0f);
};

void Element::stone()
{
    isSolid = true;
    isEmpty = false;
    color = sf::Color(
        sf::Uint8(105),
        sf::Uint8(105),
        sf::Uint8(105),
        sf::Uint8(199) + rand() % 56
    );
}

void Element::sand()
{
    isSolid = true;
    isEmpty = false;
    isMovable = true;
    color = sf::Color(
    sf::Uint8(255),
    sf::Uint8(200 + rand() % 56),
    sf::Uint8(100 + rand() % 56),
    sf::Uint8(255)
    );
}

void Element::water()
{
    isLiquid = true;
    isEmpty = false;
    isMovable = true;
    viscosity = 1.0;
    color = sf::Color(
    sf::Uint8(0),
    sf::Uint8(150 + rand() % 56),
    sf::Uint8(200 + rand() % 46),
    sf::Uint8(255)
    );
}
