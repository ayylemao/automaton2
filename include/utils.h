#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <tuple>
#include <cmath>
#include <algorithm>

namespace utils
{
    bool coinToss();
    std::vector<int> shuffleXOrder(int const grid_x);
    std::vector<std::tuple<int, int>> bresenhamLine(int x1, int y1, int x2, int y2);
}