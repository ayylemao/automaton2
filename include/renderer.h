#pragma once
#include <SFML/Graphics.hpp>
#include "../include/grid.h"

class Renderer
{
    public:
        Renderer(sf::RenderWindow& window_r, Grid& g, float const width, float const height);
        sf::Texture gridTexture;
        sf::Sprite gridSprite;
        void drawGrid();
        void setMargin(float s_margin);
        float getCellSize();
        sf::RenderWindow& window;
        sf::Clock renderClock;
        sf::VertexArray cell;

    private:
        Grid& grid;
        float const window_width;
        float const window_height;
        float margin;
        float cellSize;
};
