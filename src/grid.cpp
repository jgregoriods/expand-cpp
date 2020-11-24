#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "grid.h"

Grid::Grid(int height, int width) : height(height), width(width) {
    std::vector<std::vector<int>> agents_v(height, std::vector<int> (width, 0));
    this->agents = agents_v;

    std::vector<std::vector<int>> owner_v(height, std::vector<int> (width, 0));
    this->owner = owner_v;

    std::vector<std::vector<int>> arrival_v(height, std::vector<int> (width, -1));
    this->arrival = arrival_v;

    this->elevation = add_layer("layers/ele.asc");
    this->suit = add_layer("layers/suit.asc");
    this->veg = add_layer("layers/veg5000.asc");
}

Grid::Grid() : height(825), width(638) {}

std::vector<std::vector<double>> Grid::add_layer(std::string filename) {
    std::vector<std::vector<double>> v;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        // Skip header
        for (int i {0}; i < 6; ++i) {
            std::getline(file, line);
        }
        while (std::getline(file, line)) {
            v.push_back(std::vector<double>());
            std::stringstream split(line);
            double value;
            while (split >> value)
                v.back().push_back(value);
        }
    }
    file.close();
    return v;
}

const double MIN_X {-2985163.8955};
const double MAX_Y {5227968.786};
const int CELL_SIZE {10000};

std::pair<int, int> Grid::to_grid(double x, double y) {
    int grid_x {}, grid_y {};
    grid_x = round((x - MIN_X) / CELL_SIZE);
    grid_y = abs(round((y - MAX_Y) / CELL_SIZE));
    return std::make_pair(grid_x, grid_y);
}

std::pair<double, double> Grid::to_albers(int x, int y) {
    double albers_x {MIN_X + x * CELL_SIZE};
    double albers_y {MAX_Y - y * CELL_SIZE};
    return std::make_pair(albers_x, albers_y);
}