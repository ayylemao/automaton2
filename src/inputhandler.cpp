#include "../include/inputhandler.h"

InputHandler::InputHandler(Grid &g, Renderer &r) : grid(g), renderer(r)
{
    cellSize = renderer.getCellSize();
    lastKeyPressed = sf::Keyboard::Key::Q;
    markerRadius = 4;
}

void InputHandler::clickDrawEvent(sf::Event &event)
{
    int markerRadius = 4;
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(renderer.window);
        int gridX = mousePosition.x / renderer.getCellSize();
        int gridY = mousePosition.y / renderer.getCellSize();

        for (int i = 0; i<markerRadius; i++)
        {
            for (int j = 0; j<markerRadius; j++)
            {
                if (grid.isInBoundary(gridX + i, gridY + j))
                {
                    Element stone;
                    stone.stone();
                    grid.setElement(stone, gridX + i, gridY + j);
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(renderer.window);
        int gridX = mousePosition.x / renderer.getCellSize();
        int gridY = mousePosition.y / renderer.getCellSize();
 
        for (int i = 0; i<markerRadius; i++)
        {
            for (int j = 0; j<markerRadius; j++)
            {
                if (grid.isInBoundary(gridX + i, gridY + j))
                {
                    Element sand;
                    sand.sand();
                    grid.setElement(sand, gridX + i, gridY + j);
                }
            }
        }
    }
}


void InputHandler::drawStaticElement(sf::Event& event) {
    static bool isDrawing = false;
    static int startX;
    static int startY;
    int startGridX;
    int startGridY;
    static float elapsed = 0;

    if (isDrawing)
    {
		elapsed += renderer.renderClock.getElapsedTime().asSeconds() - elapsed;
    }
    
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        isDrawing = true;
        startX = event.mouseButton.x;
        startY = event.mouseButton.y;
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDrawing = false;
        elapsed = 0;
    }

	if (isDrawing) {
        int endX = sf::Mouse::getPosition(renderer.window).x;
        int endY = sf::Mouse::getPosition(renderer.window).y;
        startGridX = startX / cellSize;
        startGridY = startY / cellSize;
        endX = endX / cellSize;
        endY = endY / cellSize;
        std::vector<std::tuple<int, int>> linePixels = utils::bresenhamLine(startGridX, startGridY, endX, endY);


        for (const auto& pixel : linePixels) {
            int x = std::get<0>(pixel);
            int y = std::get<1>(pixel);
            if (grid.isInBoundary(x, y))
            {
                drawCircle(x, y, markerRadius);
            }

        }
		if (elapsed > 0.01)
			{
				startX = sf::Mouse::getPosition(renderer.window).x;
				startY = sf::Mouse::getPosition(renderer.window).y;
				elapsed = 0;
			} 

        renderer.window.clear();
        renderer.drawGrid();
        renderer.window.display();
	}
}

void InputHandler::spawnDynamicElement()
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(renderer.window);
	int x = mouse_pos.x / cellSize;
	int y = mouse_pos.y / cellSize;
	if (grid.isInBoundary(x, y))
	{
		drawCircle(x, y, markerRadius);
	}
}

void InputHandler::drawCircle(int x, int y, int r)
{
    for (int i = x - r; i <= x + r; ++i) {
        for (int j = y - r; j <= y + r; ++j) {
            // Calculate the distance from (x, y) to (i, j)
            int distanceSquared = (i - x) * (i - x) + (j - y) * (j - y);

            if (distanceSquared <= r * r) {
                if (lastKeyPressed == sf::Keyboard::Key::Q)
                {
                    if (grid.isInBoundary(i, j))
                    {
                        Element sand;
                        sand.sand();
                        grid.setElement(sand, i, j);
                    }
                }
                else if (lastKeyPressed == sf::Keyboard::Key::W)
                {
                    if (grid.isInBoundary(i, j))
                    {
                        Element stone;
                        stone.stone();
                        grid.setElement(stone, i, j);
                    }
                }
                else if (lastKeyPressed == sf::Keyboard::Key::E)
                {
                }
                else if (lastKeyPressed == sf::Keyboard::Key::R)
                {
                    if (grid.isInBoundary(i, j))
                    {
                        Element empty;
                        grid.setElement(empty, i, j);
                    }
                }
            }
        }
    }
}

void InputHandler::getLastKeyPressed(sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        lastKeyPressed = event.key.code;
    }
}

void InputHandler::changeRadius(sf::Event& event)
{
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        float delta = event.mouseWheelScroll.delta;
        if (delta > 0)
        {
            markerRadius += 1;
        }
        else if (delta < 0 && markerRadius > 0)
        {
            markerRadius -= 1;
        }
    }
}

void InputHandler::drawMouseRadius()
{
    const int numPoints = 50; 
    int x = sf::Mouse::getPosition(renderer.window).x;
    int y = sf::Mouse::getPosition(renderer.window).y;
    float r = markerRadius * cellSize;
    sf::CircleShape circle(r);
    circle.setPosition(x - r, y - r);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(1.0);

    renderer.window.draw(circle);
}