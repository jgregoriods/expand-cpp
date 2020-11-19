#include <iostream> // remove
#include <random>
#include <vector>
#include <utility>
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
        std::cout << id << " was born!" << std::endl;
}

void Agent::grow() {
    population += population * r;
    if (population > k)
        population = k;
}

Agent* Agent::fission() {
    population /= 2;
    Agent* new_agent = new Agent(*model, x, y, population,
                                 fission_threshold, k);
    return new_agent;
}

void Agent::check_fission() {
    if (population > fission_threshold) {
        std::vector<std::pair<int, int>> cells {check_empty_cells()};
        if (cells.size() > 0) {
            Agent* new_agent = fission();
            new_agent->move(cells[0].first, cells[0].second);
        }
    }
}

void Agent::move(int new_x, int new_y) {
    if (model->grid.agents[y][x] == id)
        model->grid.agents[y][x] = 0;
    x = new_x;
    y = new_y;
    model->grid.agents[new_y][new_x] = id;
    std::cout << id << " moved to " << new_x << ' ' << new_y << std::endl;
}

void Agent::step() {
    grow();
    check_fission();
}

std::vector<std::pair<int, int>> Agent::check_empty_cells() {
    std::vector<std::pair<int, int>> cells;
    for (int i {-1}; i <= 1; ++i) {
        for (int j {-1}; j <= 1; ++j) {
            // no need for first condition, remove it later
            if (!(x+i == x && y+j == y) && model->grid.agents[y+j][x+i] == 0)
                cells.push_back(std::make_pair(x+i, y+j));
        }
    }
    return cells;
}