#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "grid.h"

/**
 * Implementation of the model's grid.
 */
Grid::Grid(int height, int width, int start) :
    height(height),
    width(width) {
        agents = new_layer(0);
        owner = new_layer(0);
        arrival = new_layer(-1);
        elevation = layer_from_file("layers/ele.asc");
        suitability = layer_from_file("layers/suitability.asc");
}

Grid::Grid() : height(825), width(638) {}

/**
 * Reads an asc file with elevation, suitability or other environmental
 * layer and returns it as a two-dimensional vector that can be stored as
 * a layer in the grid.
 * 
 * @param filename The name of the file to be read.
 * @return a two-dimensional vector with the cell values.
 */
std::vector<std::vector<double>> Grid::layer_from_file(std::string filename) {
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

/**
 * Creates an empty two-dimensional vector that can be stored as a layer.
 * 
 * @param val The value for all cells in the layer.
 * @return a two-dimensional vector.
 */
std::vector<std::vector<int>> Grid::new_layer(int val) {
    std::vector<std::vector<int>> v(height, std::vector<int>(width, val));
    return v;
}

// These values are taken from the header of the asc files with the
// environmental layers and are used for converting to grid coords.
const double MIN_X {-2985163.8955};
const double MAX_Y {5227968.786};
const int CELL_SIZE {10000};

/**
 * Converts a pair of coordinates in South America Albers Equal Area Conic
 * projection to the grid's coordinates.
 * 
 * @param x The x coordinate in Albers projection.
 * @param y The y coordinate in Albers projection.
 * @return a pair of grid coordinates.
 */
std::pair<int, int> Grid::to_grid(double x, double y) {
    int grid_x {}, grid_y {};
    grid_x = round((x - MIN_X) / CELL_SIZE);
    grid_y = abs(round((y - MAX_Y) / CELL_SIZE));
    return std::make_pair(grid_x, grid_y);
}

/**
 * Converts a pair of grid coordinates to South America Albers Equal Area Conic
 * projection.
 * 
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return a pair of (x, y) coordinates in Albers projection.
 */
std::pair<double, double> Grid::to_albers(int x, int y) {
    double albers_x {MIN_X + x * CELL_SIZE};
    double albers_y {MAX_Y - y * CELL_SIZE};
    return std::make_pair(albers_x, albers_y);
}
