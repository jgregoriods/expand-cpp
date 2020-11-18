#include <iostream> // remove
#include "model.h"

Model::Model() {
    Grid new_grid;
    grid = new_grid;
}

void Model::step() {
    int n_agents = agents.size();
    std::cout << std::endl << n_agents << std::endl;
    for (int i {0}; i < n_agents; ++i) {
        std::cout << agents[i]->id << ' ';
        agents[i]->step();
    }
}