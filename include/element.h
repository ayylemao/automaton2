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
    sf::Color color = sf::Color(0, 0, 0, 0);
    Element();
    void stone();
    void sand();
    void water();
};
