#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
    public:
        int height, width;
        std::vector<std::vector<int>> agents, arrival, owner;
        std::vector<std::vector<double>> elevation, suit;
        Grid(int height, int width);
        Grid();
        std::vector<std::vector<double>> add_layer(std::string filename);
};

#endif