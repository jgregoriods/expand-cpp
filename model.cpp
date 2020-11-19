#include <iostream> // remove
#include "model.h"

Model::Model(int start_date) : bp(start_date) {
    Grid new_grid(825, 638);
    grid = new_grid;
    agents.reserve(500000);
}

void Model::step() {
    int n_agents = agents.size();
    for (int i {0}; i < n_agents; ++i) {
        agents[i]->step();
    }
    bp--;
}