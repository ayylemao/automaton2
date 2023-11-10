#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

struct Element
{
    bool isSolid = false;
    bool isGas = false;
    bool isLiquid = false;
    bool isMovable = false;
    bool isEmpty = true;
    bool hasMoved = false;
    sf::Color color = sf::Color(0, 0, 0, 0);
    sf::Vector2f vel;
    Element();
    void stone();
    void sand();
    void water();
};
