#include <iostream> // remove
#include <random>
#include "agent.h"
#include "model.h"

int Agent::new_id {1};

std::random_device seeder;
std::mt19937 engine(seeder());
std::uniform_int_distribution<int> dist(0, 638);

Agent::Agent(Model& model, int x, int y, int population,
             int fission_threshold, int k) :
    id(new_id++),
    model(&model),
    x(x),
    y(y),
    population(population),
    k(k),
    r(0.025),
    fission_threshold(fission_threshold) {
        if (model.grid.agents[y][x] == 0)
            model.grid.agents[y][x] = id;
        model.agents.push_back(this);
}

void Agent::grow() {
    population += population * r;
    if (population > k)
        population = k;
}

void Agent::fission() {
    if (population > fission_threshold) {
        population /= 2;
        Agent* new_agent = new Agent(*model, x, y, population, fission_threshold, k);
    }
}

void Agent::move(int new_x, int new_y) {
    if (model->grid.agents[y][x] == id)
        model->grid.agents[y][x] = 0;
    x = new_x;
    y = new_y;
    model->grid.agents[new_y][new_x] = id;
}

void Agent::step() {
    grow();
    fission();
}