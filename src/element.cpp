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
    density = 1.5f;
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
    viscosity = 1.5f;
    fluidity = 0.9f;
    density = 1.0f;
    color = sf::Color(
        sf::Uint8(0),
        sf::Uint8(150 + rand() % 56),
        sf::Uint8(200 + rand() % 46),
        sf::Uint8(255)
    );
}

void Element::lava()
{
    isLiquid = true;
    isEmpty = false;
    isMovable = true;
    viscosity = 0.5f;
    fluidity = 0.1f;
    density = 2.4f;
    color = sf::Color(
        sf::Uint8(230 + rand() % 26), // Red component (bright red to slightly darker)
        sf::Uint8(120 + rand() % 36), // Green component (for the orange tint)
        sf::Uint8(0),                 // Blue component (none for lava)
        sf::Uint8(255)                // Alpha (fully opaque)
    );
}