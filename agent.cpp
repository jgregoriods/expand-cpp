#include <iostream> // remove
#include <random>
#include <vector>
#include <utility>
#include <cmath>
#include "agent.h"
#include "model.h"

int Agent::new_id {1};

std::random_device seeder;
std::mt19937 engine(seeder());

Agent::Agent(Model& model, int x, int y, int population,
             int fission_threshold, int k, int permanence, int leap_distance) :
    id(new_id++), model(&model), x(x), y(y), r(0.025), population(population),
    k(k), fission_threshold(fission_threshold), permanence(permanence),
    time_here(0), leap_distance(leap_distance) {
        if (model.grid.agents[y][x] == 0)
            model.grid.agents[y][x] = id;
        model.agents.push_back(this);
}

void Agent::grow() {
    population += population * r;
    if (population > k)
        population = k;
}

Agent* Agent::fission() {
    population /= 2;
    Agent* new_agent = new Agent(*model, x, y, population,
                                 fission_threshold, k, permanence,
                                 leap_distance);
    return new_agent;
}

void Agent::check_fission() {
    if (population > fission_threshold) {
        std::vector<std::pair<int, int>> cells {check_empty_cells()};
        if (cells.size() > 0) {
            //std::uniform_int_distribution<int> dist(0, cells.size() - 1);
            //int cell {dist(engine)};
            std::pair<int, int> best_cell = get_best_cell(cells);
            Agent* new_agent = fission();
            new_agent->move(best_cell.first, best_cell.second);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations {check_destinations()};
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                Agent* new_agent = fission();
                new_agent->move(best_cell.first, best_cell.second);
            }
        }
    }
}

void Agent::move(int new_x, int new_y) {
    if (model->grid.agents[y][x] == id)
        model->grid.agents[y][x] = 0;
    x = new_x;
    y = new_y;
    model->grid.agents[new_y][new_x] = id;
    if (model->grid.arrival[new_y][new_x] == -1)
        model->grid.arrival[new_y][new_x] = model->bp;
}

void Agent::check_move() {
    if (time_here > permanence) {
        std::vector<std::pair<int, int>> cells {check_empty_cells()};
        if (cells.size() > 0) {
            //std::uniform_int_distribution<int> dist(0, cells.size() - 1);
            //int cell {dist(engine)};
            std::pair<int, int> best_cell = get_best_cell(cells);
            move(best_cell.first, best_cell.second);
            time_here = 0;
        }
    }
}

void Agent::step() {
    grow();
    check_fission();
    check_move();
    time_here++;
}

std::pair<int, int> Agent::get_best_cell(std::vector<std::pair<int, int>> cells) {
    double best_val {-1};
    std::pair<int, int> best_cell;
    for (int i {0}; i < cells.size(); ++i) {
        double val = model->grid.suit[cells[i].second][cells[i].first];
        if (val > best_val) {
            best_val = val;
            best_cell = cells[i];
        }
    }
    return best_cell;
}

std::vector<std::pair<int, int>> Agent::check_empty_cells() {
    std::vector<std::pair<int, int>> cells;
    for (int i {-1}; i <= 1; ++i) {
        for (int j {-1}; j <= 1; ++j) {
            // no need for first condition, remove it later
            if (model->grid.agents[y+j][x+i] == 0
                && model->grid.elevation[y+j][x+i] >= 1)
                cells.push_back(std::make_pair(x+i, y+j));
        }
    }
    return cells;
}

std::vector<std::pair<int, int>> Agent::check_destinations() {
    std::vector<std::pair<int, int>> cells;
    for (int i {-leap_distance}; i <= leap_distance; ++i) {
        for (int j {-leap_distance}; j <= leap_distance; ++j) {
            // no need for first condition, remove it later
            if (get_distance(x+i, y+j) == leap_distance
                && model->grid.agents[y+j][x+i] == 0
                && model->grid.elevation[y+j][x+i] >= 1)
                cells.push_back(std::make_pair(x+i, y+j));
        }
    }
    return cells;
}

int Agent::get_distance(int x_i, int y_i) {
    return round(sqrt(pow(x - x_i, 2) + pow(y - y_i, 2)));
}