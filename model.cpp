#include <iostream> // remove
#include "model.h"

Model::Model() {
    Grid new_grid;
    grid = new_grid;
    agents.reserve(500000);
}

void Model::step() {
    int n_agents = agents.size();
    for (int i {0}; i < n_agents; ++i) {
        agents[i]->step();
    }
}