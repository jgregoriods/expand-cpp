#include <iostream> // remove
#include "model.h"

Model::Model() {
    Grid new_grid;
    grid = new_grid;
    agents.reserve(500000);
}

void Model::step() {
    int n_agents = agents.size();
    //std::cout << std::endl << n_agents << std::endl;
    for (int i {0}; i < n_agents; ++i) {
        agents[i]->step();
        //std::cout << agents[i]->population << ' ';
    }
}