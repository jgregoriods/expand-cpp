#ifndef GRID_H
#define GRID_H

#include <utility>
#include <vector>

class Grid {
    public:
        Grid(int height, int width);
        Grid();
        std::vector<std::vector<double>> layer_from_file(std::string filename);
        std::vector<std::vector<int>> new_layer(int val);
        std::pair<int, int> to_grid(double x, double y);
        std::pair<double, double>to_albers(int x, int y);
        int height;
        int width;
        std::vector<std::vector<int>> agents;
        std::vector<std::vector<int>> arrival;
        std::vector<std::vector<int>> owner;
        std::vector<std::vector<double>> elevation;
        std::vector<std::vector<double>> suitability;
        std::vector<std::vector<double>> vegetation;
};

#endif