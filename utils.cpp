#include <utility>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iomanip>

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

std::map<int, double> read_dates(std::string filename) {
    std::string line;
    std::map<int, double> dates;
    //double x, y;
    int year;
    double prob;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::stringstream split(line);
            split >> year >> prob;
            dates.insert(std::make_pair(year, prob));
        }
    }
    return dates;
}

int main() {
    std::cout << std::fixed << std::setprecision(4);
    auto a {read_dates("dates/Boa Vista.txt")};
    int i {150};
    if (a.find(i) == a.end())
        std::cout << "not found" << std::endl;
    else
        std::cout << a[i] << std::endl;
    return 0;
}