#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
    public:
        std::vector<std::vector<int>> agents;
        std::vector<std::vector<double>> elevation;
        std::vector<std::vector<double>> suit;
        Grid();
        std::vector<std::vector<double>> add_layer(std::string filename);
};

#endif