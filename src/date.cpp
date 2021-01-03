#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "date.h"

Date::Date(std::string filename) : prob {0.0}, year {-1} {
    read_file(filename);
}

/*
* Given a file whose first line is a pair of coordinates and the remaining
* lines are years bp and calibrated probabilities, updates the object's
* x, y and probs (year-probability map).
*/
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
        file.close();
    }
}

double Date::get_x() {
    return x;
}

double Date::get_y() {
    return y;
}

void Date::set_prob(int year) {
    if (probs.find(year) != probs.end())
        prob = probs[year];
}

double Date::get_prob() {
    return prob;
}