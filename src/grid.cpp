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
    size_t index = y * size_x + x;
    grid[index] = element;
}

void Grid::swapElement(size_t x1, size_t y1, size_t x2, size_t y2)
{
    size_t index1 = from_2d(x1, y1);
    size_t index2 = from_2d(x2, y2);
    grid[index1].hasMoved = true;
    std::swap(grid[index1], grid[index2]);
}

Element& Grid::getElement(size_t x, size_t y)
{
    size_t index = y * size_x + x;
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
            updateCell(rowOffset + columnOffset, columnOffset, row);
        }
    }

    for (Element& ele : grid)
    {
        ele.hasMoved = false;
    }
}

void Grid::updateVelocity(size_t index)
{
    Element& element = grid[index];
    if (element.vel.y > maxSpeed)
    {
        element.vel.y = utils::sign(element.vel.y) * maxSpeed;
    }
    else
    {
        element.vel.y += g;
    }
}

void Grid::resetVelocity(size_t index)
{
    grid[index].vel.y = 0.0f;
}

int Grid::getVelUpdateCountY(size_t index, float velY)
{
    float absVelocity = std::abs(velY);
    int floored = std::floor(absVelocity);
    float mod = absVelocity - floored;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return floored + (dist(eng) < mod ? 1 : 0);
}

int Grid::getVelUpdateCountX(size_t index, float velX)
{
    float absVelocity = std::abs(velX);
    int floored = std::floor(absVelocity);
    float mod = absVelocity - floored;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return floored + (dist(eng) < mod ? 1 : 0);
}

void Grid::updateCell(size_t index, size_t x, size_t y)
{
    Element& currElement = getElement(x, y);
    if (currElement.isMovable && !currElement.hasMoved)
    {
        if (currElement.isSolid) 
        { 
            updateMovableSolid(currElement, index, x, y); 
            return;
        }
        else if (currElement.isLiquid)
        {
            updateLiquid(currElement, index, x, y);
        }
        

    }
}

void Grid::updateLiquid(Element& currElement, size_t index, size_t x, size_t y)
{
    int y_down = y + 1;
    CellState downState = state(x, y_down);
    CellState leftState = state(x-1, y);
    CellState rightState = state(x+1, y);
    if (downState != CellState::Empty && leftState != CellState::Empty && rightState != CellState::Empty)
    { return; }
    if (downState == CellState::Empty)
    {
        int updateCount = getVelUpdateCountY(index, currElement.vel.y);
        if (updateCount < 1)
        {
            updateVelocity(index);
            swapElement(x, y, x, y_down);
            return;
        }
        CellState checkFurther = CellState::Empty;
        for (int update = 0; update < updateCount; update++) {
            y_down = y_down + 1;
            checkFurther = state(x, y_down);
            if (checkFurther == CellState::OutOfBounds || checkFurther == CellState::Solid) {
                break;
            }
        }
        if (checkFurther == CellState::OutOfBounds) {setElement(Element(), x, y); return;}
        else if (checkFurther == CellState::Solid)
        {
            if (getElement(x, y_down).vel.y == 0.0f)
            {
                currElement.vel.x = utils::coinToss() ? -velConv * currElement.vel.y : velConv * currElement.vel.y;
                resetVelocity(index);
            }
            swapElement(x, y, x, y_down - 2);
            return;
        }
        else if (checkFurther == CellState::Liquid)
        {
            resetVelocity(index);
            swapElement(x, y, x, y_down - 2);
        }
        else if (checkFurther == CellState::Empty)
        {
            updateVelocity(index);
            swapElement(x, y, x, y_down);
            return;
        }
    }
    if (downState == CellState::OutOfBounds)
    {
        setElement(Element(), x, y);
        return;
    }
    if (downState == CellState::Solid || downState == CellState::Liquid)
    {
        CellState left = state(x - 1, y);
        CellState right = state(x + 1, y);
        DiagEnum diag = checkHori(left, right);
        int updateCount;
        if (std::abs(currElement.vel.x) > 0.0f) { updateCount = getVelUpdateCountX(index, currElement.vel.x); }
        else
        {
            if (diag == DiagEnum::Left) 
            { 
                currElement.vel.x = -4*velConv;
                swapElement(x, y, x-1, y); 
                return;
            }
            else if (diag == DiagEnum::Right)
            {
                currElement.vel.x = +4*velConv;
                swapElement(x, y, x+1, y); 
                return;
            }
        }
        for (int i = 0; i < updateCount; i++)
        {
            size_t x_horizontal = x + utils::sign(currElement.vel.x) * (i+1);
            CellState horizontalCell = state(x_horizontal, y);
            CellState downCell = state(x_horizontal, y+1);
            if (horizontalCell == CellState::Empty && downCell == CellState::Empty)
            {
                currElement.vel.x = 0.0f;
                swapElement(x, y, x_horizontal, y);
                return;
            }
            else if (horizontalCell == CellState::Solid || horizontalCell == CellState::OutOfBounds)
            {
                currElement.vel.x = 0.0f;
                swapElement(x, y, x + updateCount * utils::sign(currElement.vel.x), y);
                return;
            }
            //currElement.vel.x *= currElement.viscosity;
            swapElement(x, y, x + (updateCount) * utils::sign(currElement.vel.x), y);
        }
        return;
    }
}

void Grid::updateMovableSolid(Element& currElement, size_t index, size_t x, size_t y)
{
    //int y_down = y + 1;
    //CellState downState = state(x, y_down);
    //switch (downState)
    //{
    //    case CellState::Empty:
    //    {
    //        int updateCount = getVelUpdateCountY(index, currElement.vel.y);
    //        if (updateCount < 1)
    //        {
    //            updateVelocity(index);
    //            swapElement(x, y, x, y_down);
    //            return;
    //        }
    //        CellState checkFurther = CellState::Empty;
    //        for (int update = 0; update < updateCount; update++) {
    //            y_down = y_down + 1;
    //            checkFurther = state(x, y_down);
    //            if (checkFurther == CellState::OutOfBounds || checkFurther == CellState::Solid) {
    //                break;
    //            }
    //        }
    //        if (checkFurther == CellState::OutOfBounds) { }
    //        else if (checkFurther == CellState::Solid)
    //        {
    //            if (getElement(x, y_down).vel.y == 0.0f)
    //            {
    //                currElement.vel.x = utils::coinToss() ? -velConv * currElement.vel.y : velConv * currElement.vel.y;
    //                resetVelocity(index);
    //            }
    //            swapElement(x, y, x, y_down - 1);
    //            return;
    //        }
    //        else if (checkFurther == CellState::Liquid)
    //        {
    //            resetVelocity(index);
    //            swapElement(x, y, x, y_down - 2);
    //        }
    //        else if (checkFurther == CellState::Empty)
    //        {
    //            updateVelocity(index);
    //            swapElement(x, y, x, y_down);
    //            return;
    //        }
    //    }
    //    case CellState::OutOfBounds:
    //        setElement(Element(), x, y);
    //        return;
    //    case CellState::Liquid:
    //    {
    //        swapElement(x, y, x, y_down);
    //    }
    //    case CellState::Solid:
    //    {
    //        CellState leftDiag = state(x - 1, y_down);
    //        CellState rightDiag = state(x + 1, y_down);
    //        DiagEnum diag = checkDiag(leftDiag, rightDiag);
    //        if (diag == DiagEnum::Left) 
    //        { 
    //            currElement.vel.x = -4*velConv;
    //            swapElement(x, y, x-1, y_down); 
    //        }
    //        else if (diag == DiagEnum::Right)
    //        {
    //            currElement.vel.x = +4*velConv;
    //            swapElement(x, y, x+1, y_down); 
    //        }
    //        else if (diag == DiagEnum::None)
    //        {
    //            int updateCount;
    //            if (std::abs(currElement.vel.x) > 0.0f) { updateCount = getVelUpdateCountX(index, currElement.vel.x); }
    //            else { return; }
    //            if (updateCount < 1)
    //            {
    //                currElement.vel.x = 0;
    //                return;
    //            }
    //            for (int i = 0; i < updateCount; i++)
    //            {
    //                size_t x_horizontal = x + utils::sign(currElement.vel.x) * (i+1);
    //                CellState horizontalCell = state(x_horizontal, y);
    //                CellState downCell = state(x_horizontal, y+1);
    //                if (horizontalCell == CellState::Empty && downCell == CellState::Empty)
    //                {
    //                    currElement.vel.x = 0.0f;
    //                    swapElement(x, y, x_horizontal, y);
    //                }
    //                else if (horizontalCell == CellState::Solid || horizontalCell == CellState::OutOfBounds)
    //                {
    //                    currElement.vel.x = 0.0f;
    //                    swapElement(x, y, x_horizontal - utils::sign(currElement.vel.x), y);
    //                }
    //            }
    //            currElement.vel.x *= currElement.friction;
    //            swapElement(x, y, x + (updateCount+1) * utils::sign(currElement.vel.x), y);
    //        }
    //        return;
    //    }
    //        break;
    //    default:
    //        break;
    //}
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

    if (left_cell == CellState::Empty || left_cell == CellState::Liquid) { left = true; }
    if (right_cell == CellState::Empty || right_cell == CellState::Liquid) { right = true; }
    if (left && right)
    {
        if(utils::coinToss()) { return DiagEnum::Left; }
        else { return DiagEnum::Right; }
    }
    else if (left && !right) { return DiagEnum::Left; }
    else if (!left && right) { return DiagEnum::Right; }
    else { return DiagEnum::None; }
}

DiagEnum Grid::checkHori(CellState left_cell, CellState right_cell)
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