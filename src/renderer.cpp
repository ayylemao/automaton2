#include "../include/renderer.h"


Renderer::Renderer(sf::RenderWindow& window_r, Grid& g, float const width, float const height) 
: window(window_r), grid(g), window_width(width), window_height(height)
{
    margin = 0;
    cellSize = (std::min(width, height) - 2 * margin) / std::min(grid.size_x, grid.size_x);
    cell = sf::VertexArray(sf::Quads, 4);
}

void Renderer::setMargin(float s_margin)
{
    margin = s_margin;
    cellSize = (std::min(window_width, window_height) - 2 * margin) / std::min(grid.size_x, grid.size_y);
}

float Renderer::getCellSize()
{
    return cellSize;
}

void Renderer::drawGrid()
{
    for (size_t x = 0; x < grid.size_x; x++)
    {
        for (size_t y = 0; y < grid.size_y; y++)
        {
            if (!grid.getElement(x, y).isEmpty)
            {
                sf::Color color = grid.getElement(x, y).color;
                float x_pos = static_cast<float>(x * cellSize);
                float y_pos = static_cast<float>(y * cellSize);
                cell[0].position = sf::Vector2f(x_pos, y_pos + cellSize);
                cell[1].position = sf::Vector2f(x_pos, y_pos);
                cell[2].position = sf::Vector2f(x_pos + cellSize, y_pos);
                cell[3].position = sf::Vector2f(x_pos + cellSize, y_pos + cellSize);
                cell[0].color = color;
                cell[1].color = color;
                cell[2].color = color;
                cell[3].color = color;
                window.draw(cell);
            }
        }
    }
}