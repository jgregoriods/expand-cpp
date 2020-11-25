#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "date.h"

Date::Date(std::string filename) {
    read_file(filename);
}

void Date::read_file(std::string filename) {
    std::string line {};
    int year {};
    double prob {};
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, line);
        std::stringstream split_coords(line);
        split_coords >> x >> y;
        while (std::getline(file, line)) {
            std::stringstream split(line);
            split >> year >> prob;
            probs.insert(std::make_pair(year, prob));
        }
    }
}

double Date::get_x() {
    return x;
}

double Date::get_y() {
    return y;
}

double Date::get_prob(int year) {
    if (probs.find(year) != probs.end())
        return probs[year];
    else
        return 0.0;
}