#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "grid.h"

Grid::Grid() {
    std::vector<std::vector<int>> v(825, std::vector<int> (638, 0));
    this->agents = v;
    this->elevation = add_layer("ele.asc");
    this->suit = add_layer("suit.asc");
}

std::vector<std::vector<double>> Grid::add_layer(std::string filename) {
    std::string line;
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