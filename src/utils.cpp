#include "../include/utils.h"


bool utils::coinToss()
{
    return std::rand()&1;
};


std::vector<int> utils::shuffleXOrder(int const grid_x)
{
    std::vector<int> xOrder(grid_x);
    for (int i = 0; i < grid_x; i++)
    {
        xOrder[i] = i;
    }
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(xOrder.begin(), xOrder.end(), gen);
    return xOrder;
};

std::vector<std::tuple<int, int>> utils::bresenhamLine(int x1, int y1, int x2, int y2) 
{
	std::vector<std::tuple<int, int>> linePixels;

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;

	int maxPoints = dx + dy + 1;
	linePixels.reserve(std::max(maxPoints, 5));
	while (true) {
		linePixels.emplace_back(std::make_tuple(x1, y1));

		if (x1 == x2 && y1 == y2) {
			break;
		}

		int e2 = 2 * err;

		if (e2 > -dy) 
        {
			err -= dy;
			x1 += sx;
		}

		if (e2 < dx) {
			err += dx;
			y1 += sy;
		}
	}

	return linePixels;
};


int utils::sign(float value) 
{
    if (value > 0.0f) return 1;
    if (value < 0.0f) return -1;
    return 0;
};