#include <iostream> // remove
#include <string>
#include <fstream>
#include "model.h"

Model::Model(int start_date) : bp(start_date) {
    Grid new_grid(825, 638);
    grid = new_grid;
    agents.reserve(500000);
}

void Model::update_env() {
    if (bp % 50 == 0) {
        grid.veg.clear();
        std::string filename {"layers/veg$year.asc"};
        filename.replace(filename.find("$year"), sizeof("$year") - 1, std::to_string(bp));
        grid.veg = grid.add_layer(filename);
    }
}

void Model::step() {
    auto it = agents.begin();
    while (it != agents.end()) {
        Agent* agent = *it;
        if (!agent->is_alive()) {
            delete agent;
            it = agents.erase(it);
        } else {
            agent->step();
            ++it;
        }
    }
    bp--;
    update_env();
    write();
}

void Model::run(int n) {
    int progress {};
    int k {};
    for (int i {0}; i < n; ++i) {
        step();
        k++;
        progress = ((double)k / (double)n) * 100;
        std::cout << "\r" << '[' << std::string(progress / 2, '#')
                  << std::string(50 - (progress / 2), ' ')
                  << "] " << progress << "%";
        std::cout.flush();
    }
    std::cout << std::endl;
}

void Model::write() {
    std::string filename {"snapshots/$year.csv"};
    filename.replace(filename.find("$year"), sizeof("$year") - 1, std::to_string(bp));
    std::ofstream file;
    file.open(filename);
    for (int i {0}; i < agents.size() ; ++i) {
        file << agents[i]->get_x() << ", " << agents[i]->get_y() << "\n";
    }
    file.close();
}