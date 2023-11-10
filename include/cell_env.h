#pragma once
#include "../include/grid.h"
#include "../include/element.h"

enum class CellState {
    OutOfBounds,
    Empty,
    Solid,
    Liquid,
    Other
};

enum class DiagEnum
{
    Left,
    Right,
    Both,
    None,
    Other
};