#include <iostream>
#include "../include/grid.h"


int main()
{
    auto grid = Grid(50, 50);
    grid.swapElement(1, 2, 2, 4); 
    return 0;
};