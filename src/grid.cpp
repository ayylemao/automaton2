#include "../include/grid.h"


Grid::Grid(size_t x, size_t y) : size_x(x), size_y(y), eng(rd())
{
    grid.reserve(size_x * size_y);
    for (size_t i = 0; i < size_x * size_y; i++)
    {
        grid.emplace_back();
    }
};

bool Grid::isInBoundary(int x, int y)
{
    return (x >= 0 && x < static_cast<int>(size_x)) && (y >= 0 && y < static_cast<int>(size_y));
}

size_t Grid::from_2d(int x, int y)
{
    return y * size_x + x;
}

sf::Vector2i Grid::to_2d(size_t index)
{
    return sf::Vector2i(index % size_x, index / size_x);
}

void Grid::setElement(Element element, size_t x, size_t y)
{
    grid[from_2d(x, y)] = element;
}

void Grid::swapElement(size_t x1, size_t y1, size_t x2, size_t y2)
{
    size_t index1 = from_2d(x1, y1);
    size_t index2 = from_2d(x2, y2);
    std::swap(grid[index1], grid[index2]);
}

Element& Grid::getElement(size_t x, size_t y)
{
    size_t index = from_2d(x, y);
    return grid[index];
}

Element& Grid::getElement(size_t index)
{
    return grid[index];
}

void Grid::update()
{
    size_t rowOffset;
    size_t columnOffset;
    bool leftToRight;
    for (size_t row = size_y; row --> 0;)
    {
        rowOffset = row * size_x;
        leftToRight = utils::coinToss();
        for (size_t i = 0; i < size_x; i++)
        {
            columnOffset = leftToRight ? i : -i - 1 + size_x;
            updateCell(rowOffset + columnOffset);
        }
    }
}

void Grid::updateCell(size_t index)
{
    int y_max = 0;
    sf::Vector2i pos = to_2d(index);
    auto x = pos.x;
    auto y = pos.y;
    Element& currElement = getElement(x, y);
    if (currElement.isMovable)
    {
        if (currElement.isSolid)
        {
            int y_down = y + 1;
            CellState downState = state(x, y_down);

            if (y > y_max)
            {
                y_max = y;
                //std::cout << y_max << '\n'; 
            }

            switch (downState)
            {
                case CellState::OutOfBounds:
                    setElement(Element(), x, y);
                    return;
                case CellState::Empty:
                    swapElement(x, y, x, y_down);
                    return;
                case CellState::Solid:
                {
                    CellState leftDiag = state(x - 1, y_down);
                    CellState rightDiag = state(x + 1, y_down);
                    DiagEnum diag = checkDiag(leftDiag, rightDiag);
                    if (diag == DiagEnum::Left) { 
                        swapElement(x, y, x-1, y_down);
                        }
                    else if (diag == DiagEnum::Right) { 
                        swapElement(x, y, x+1, y_down); 
                        }
                    return;
                }
                    break;
                default:
                    break;
            }
        }
    }
}

CellState Grid::state(int x, int y) {
    
    if (!isInBoundary(x, y)) {
        return CellState::OutOfBounds;
    }
    
    Element& element = getElement(x, y);

    if (element.isEmpty)
    {
        return CellState::Empty;
    }

    if (element.isSolid)
    {
        return CellState::Solid;
    }

    if (element.isLiquid)
    {
        return CellState::Liquid;
    }

    return CellState::Other;
}

DiagEnum Grid::checkDiag(CellState left_cell, CellState right_cell)
{
    bool left = false;
    bool right = false;

    if (left_cell == CellState::Empty) { left = true; }
    if (right_cell == CellState::Empty) { right = true; }
    if (left && right)
    {
        if(utils::coinToss()) { return DiagEnum::Left; }
        else { return DiagEnum::Right; }
    }
    else if (left && !right) { return DiagEnum::Left; }
    else if (!left && right) { return DiagEnum::Right; }
    else { return DiagEnum::None; }
}
