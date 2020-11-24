#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>

class Grid {
    public:
        int height, width;
        std::vector<std::vector<int>> agents, arrival, owner;
        std::vector<std::vector<double>> elevation, suit, veg;
        Grid(int height, int width);
        Grid();
        std::vector<std::vector<double>> add_layer(std::string filename);
        std::pair<int, int> to_grid(double x, double y);
        std::pair<double, double>to_albers(int x, int y);
};

#endif