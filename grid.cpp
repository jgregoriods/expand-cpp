#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "grid.h"

Grid::Grid(int height, int width) : height(height), width(width) {
    std::vector<std::vector<int>> agents_v(height, std::vector<int> (width, 0));
    this->agents = agents_v;

    std::vector<std::vector<int>> owner_v(height, std::vector<int> (width, 0));
    this->owner = owner_v;

    std::vector<std::vector<int>> arrival_v(height, std::vector<int> (width, -1));
    this->arrival = arrival_v;

    this->elevation = add_layer("ele.asc");
    this->suit = add_layer("suit.asc");
    this->veg = add_layer("veg1000.asc");
}

Grid::Grid() : height(825), width(638) {}

std::vector<std::vector<double>> Grid::add_layer(std::string filename) {
    std::vector<std::vector<double>> v;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
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