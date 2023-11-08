#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../include/element.h"
#include "../include/utils.h"
#include "../include/cell_env.h"

class Grid
{
    public:
        size_t size_x;
        size_t size_y;
        std::vector<Element> grid;
        Grid(size_t x, size_t y);
        size_t from_2d(int x, int y);
        sf::Vector2i to_2d(size_t index);
        void setElement(Element element, size_t x, size_t y);
        void swapElement(size_t x1, size_t y1, size_t x2, size_t y2);
        bool isInBoundary(int x, int y);
        Element& getElement(size_t x, size_t y);
        Element& getElement(size_t index);
        void update();
        void updateCell(size_t index);
        CellState state(int x, int y);
};