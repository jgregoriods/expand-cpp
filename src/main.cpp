#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "agent.h"
#include "date.h"
#include "grid.h"
#include "model.h"

struct Options {
    int start_date {5000};
    int fission_threshold {100};
    int k {20};
    int permanence {10};
    int leap_distance {15};
    std::string culture {"arawak"};
    std::string origin {"lagruta"};
    bool show_bar {false};
    bool write_files {false};
    double forest {0.4};
    double maxent {0.25};
    Options(std::vector<std::string> args) {
        for (auto str: args) {
            if (str.substr(0, 7) == "--date=")
                start_date = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 7) == "--site=")
                origin = str.substr(7, str.length());
            else if (str.substr(0, 7) == "--fiss=")
                fission_threshold = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 4) == "--k=")
                k = std::stoi(str.substr(4, str.length()));
            else if (str.substr(0, 7) == "--perm=")
                permanence = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 7) == "--leap=")
                leap_distance = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 7) == "--cult=")
                culture = str.substr(7, str.length());
            else if (str.substr(0, 6) == "--veg=")
                forest = std::stod(str.substr(6, str.length()));
            else if (str.substr(0, 6) == "--max=")
                maxent = std::stod(str.substr(6, str.length()));
            else if (str == "--show-bar")
                show_bar = true;
            else if (str == "--write")
                write_files = true;

        }
    }
};

int main(const int argc, const char* argv[]) {
    std::vector<std::string> args(argv+1, argv+argc);
    Options opts(args);
    Model model(opts.culture, opts.start_date, opts.maxent, opts.forest);
    auto coords = model.get_coords(opts.origin);
    auto grid_coords = model.to_grid(coords.first, coords.second);
    model.setup(grid_coords, opts.fission_threshold, opts.k, opts.permanence, opts.leap_distance);
    model.run(opts.start_date - 600, opts.write_files, opts.show_bar);
    model.load_dates();
    std::cout << std::fixed << std::setprecision(4) << model.get_score() << std::endl;
    return 0;
}