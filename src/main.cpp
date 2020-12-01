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
    bool show_bar {false};
    bool write_files {false};
    double diffusion {0.0};
    Options(std::vector<std::string> args) {
        for (auto str: args) {
            if (str.substr(0, 7) == "--date=")
                start_date = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 7) == "--fiss=")
                fission_threshold = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 4) == "--k=")
                k = std::stoi(str.substr(4, str.length()));
            else if (str.substr(0, 7) == "--perm=")
                permanence = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 7) == "--leap=")
                leap_distance = std::stoi(str.substr(7, str.length()));
            else if (str.substr(0, 7) == "--diff=")
                diffusion = std::stod(str.substr(7, str.length()));
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
    Model model;
    model.setup(opts.start_date, 229, 76, opts.fission_threshold,
                opts.k, opts.permanence, opts.leap_distance, opts.diffusion);
    model.run(4400, opts.write_files, opts.show_bar);
    model.load_dates("dates");
    std::cout << std::fixed << std::setprecision(4) << model.get_score() << std::endl;
    return 0;
}