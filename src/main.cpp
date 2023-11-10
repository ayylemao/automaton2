#include <iostream>
#include "../include/grid.h"
#include "../include/renderer.h"
#include "../include/inputhandler.h"


int main()
{
    float frame_cap = 60.0;
    auto grid = Grid(500, 500);
    float windowWidth = 800;
    float windowHeight = 800;
    float updateInterval = 1.0/frame_cap;
    float elapsed = 0.0;
    bool mousePressLeft = false;
    bool mousePressRight = false;
    
    sf::Clock clock;  

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Automaton");
    auto renderer = Renderer(window, grid, windowWidth, windowHeight);
    auto inputhandler = InputHandler(grid, renderer);
    Element stone;
    stone.stone();
    for (size_t i = 0; i < grid.size_x; i++)
    {
        grid.setElement(stone, i, 150);
    }

    renderer.setMargin(0);


        while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) { window.close();}
                inputhandler.drawStaticElement(event);
                inputhandler.getLastKeyPressed(event);
                inputhandler.changeRadius(event);


                if (event.type == sf::Event::MouseButtonPressed &&  event.mouseButton.button == sf::Mouse::Left)
                {
                    mousePressLeft = true;
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    mousePressLeft = false;
                }

                if (event.type == sf::Event::MouseButtonPressed &&  event.mouseButton.button == sf::Mouse::Right)
                {
                    mousePressRight = true;
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    mousePressRight = false;
                }
        }
        elapsed += clock.restart().asSeconds();

        if (mousePressRight) { inputhandler.spawnDynamicElement(); }

        if (elapsed >= updateInterval && !mousePressLeft)
        {
            std::cout << 1.0/elapsed << '\n';
            elapsed = 0;
            grid.update();
        }
        window.clear(); 
        inputhandler.drawMouseRadius();
        renderer.drawGrid();
        window.display();
    }
    return 0;
};