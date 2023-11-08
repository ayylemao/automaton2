#include "../include/grid.h"


Grid::Grid(size_t x, size_t y) : size_x(x), size_y(y)
{
    grid.reserve(size_x * size_y);
    for (size_t i = 0; i < size_x * size_y; i++)
    {
        grid.emplace_back();
    }
};

bool Grid::isInBoundary(int x, int y)
{
    return (x>=0 && x < size_x) && (y>=0 && y < size_y);
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
    for (size_t index = size_x * size_y; index >= 0; --index)
    {
        updateCell(index);
    }
}

void Grid::updateCell(size_t index)
{
    sf::Vector2i pos = to_2d(index);
    auto x = pos.x;
    auto y = pos.y;
    Element& currElement = getElement(x, y);
    CellState leftDiag;
    CellState rightDiag;

    if (currElement.isMovable)
    {
        int y_down = y + 1;
        CellState downState = state(x, y_down);

        switch (downState)
        {
            case CellState::OutOfBounds:
                setElement(Element(), x, y);
                return;
            case CellState::Empty:
                swapElement(x, y, x, y_down);
                return;
            case CellState::Solid:
                leftDiag = state(x - 1, y_down);
                rightDiag = state(x + 1, y_down);
                // TODO
                break;
            default:
                break;
        }


        //Element& elementDown = getElement(x, y_down);
        //if (elementDown.isEmpty)
        //{
        //    swapElement(x, y, x, y+1);
        //    return;
        //}
        //else if (elementDown.isSolid)
        //{
        //    int left_x = x - 1;
        //    int right_x = x + 1;
        //    bool leftInBounds = isInBoundary(left_x, y_down);
        //    bool rightInBounds = isInBoundary(right_x, y_down);
        //    


        //    Element& elementLeft = getElement(next_x-1, next_y);
        //    Element& elementRight = getElement(next_x+1, next_y);
        //    if (elementLeft.isEmpty && elementRight.isEmpty)
        //    {
        //        if (utils::coinToss())
        //    }

        //}
        


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