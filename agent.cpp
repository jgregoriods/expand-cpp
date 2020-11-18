#include <iostream> // remove
#include <random>
#include "agent.h"
#include "model.h"

int Agent::new_id {1};

std::random_device seeder;
std::mt19937 engine(seeder());
std::uniform_int_distribution<int> dist(0, 638);

Agent::Agent(Model& model, int x, int y, int population,
             int fission_threshold) :
    id(new_id++),
    model(&model),
    x(x),
    y(y),
    population(population),
    fission_threshold(fission_threshold) {
        model.grid.agents[y][x] = id;
        model.agents.push_back(this);
        std::cout << id << " was born!" << std::endl;
}

void Agent::grow() {
    population += 1;
}

void Agent::fission() {
    Agent* new_agent = new Agent(*model, x, y, population, fission_threshold);
    //model->agents.push_back(new_agent);
}

void Agent::move(int new_x, int new_y) {
    model->grid.agents[y][x] = 0;
    x = new_x;
    y = new_y;
    model->grid.agents[new_y][new_x] = id;
}

void Agent::step() {
    grow();
    if (population > fission_threshold) {
        population /= 2;
        fission();
    }
    move(dist(engine), dist(engine));
}