#include <utility>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iomanip>
#include <memory>
#include <vector>

const double MIN_X {-2985163.8955};
const double MAX_Y {5227968.786};
const int CELL_SIZE {10000};

std::pair<int, int> to_grid(double x, double y) {
    int grid_x {}, grid_y {};
    grid_x = round((x - MIN_X) / CELL_SIZE);
    grid_y = abs(round((y - MAX_Y) / CELL_SIZE));
    return std::make_pair(grid_x, grid_y);
}

std::pair<double, double> to_albers(int x, int y) {
    double albers_x {MIN_X + x * CELL_SIZE};
    double albers_y {MAX_Y - y * CELL_SIZE};
    return std::make_pair(albers_x, albers_y);
}

class Test {
    public:
        int x;
        Test(int x) : x(x) {}
};

int main() {
    std::vector<std::shared_ptr<Test>> tests;
    std::shared_ptr<Test> test(new Test(11));
    tests.reserve(100);
    tests.push_back(test);
    return 0;
}