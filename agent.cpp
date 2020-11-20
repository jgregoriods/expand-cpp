#include <iostream> // remove
#include <vector>
#include <utility>
#include <cmath>
#include "agent.h"
#include "model.h"

int Agent::new_id {1};
const double SUIT_VAL {0.65};
const double FOREST_VAL {0.5};

Agent::Agent(Model& model, int x, int y, int population,
             int fission_threshold, int k, int permanence, int leap_distance) :
    id(new_id++), model(&model), x(x), y(y), r(0.025), population(population),
    k(k), fission_threshold(fission_threshold), permanence(permanence),
    time_here(0), leap_distance(leap_distance), total_k(k), alive(true) {
        if (model.grid.agents[y][x] == 0 && model.grid.owner[y][x] == 0)
            model.grid.agents[y][x] = id;
            model.grid.owner[y][x] = id;
        model.agents.push_back(this);
        land.reserve(9);
}

Agent::~Agent() {
    model->grid.agents[y][x] = 0;
    model->grid.owner[y][x] = 0;
    for (int i {0}; i < land.size(); ++i) {
        model->grid.owner[land[i].second][land[i].first] = 0;
    } 
}

void Agent::grow() {
    population += population * r;
    update_land();
}

void Agent::update_land() {
    while (population > total_k) {
        std::vector<std::pair<int, int>> cells {check_destinations(1)};
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            land.push_back(best_cell);
            model->grid.owner[best_cell.second][best_cell.first] = id;
            if (model->grid.arrival[best_cell.second][best_cell.first] == -1)
                model->grid.arrival[best_cell.second][best_cell.first] = model->bp;
            total_k += k;
        } else {
            population = total_k;
        }
    }
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
        std::vector<std::pair<int, int>> cells {check_destinations(1)};
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            Agent* new_agent = fission();
            new_agent->move(best_cell.first, best_cell.second);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations {check_destinations(leap_distance)};
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                Agent* new_agent = fission();
                new_agent->move(best_cell.first, best_cell.second);
            }
        }
    }
}

void Agent::move(int new_x, int new_y) {
    if (model->grid.agents[y][x] == id) {
        model->grid.agents[y][x] = 0;
        model->grid.owner[y][x] = 0;
        for (int i {0}; i < land.size(); ++i) {
            model->grid.owner[land[i].second][land[i].first] = 0;
        }
        land.clear();
        total_k = k;
    }
    x = new_x;
    y = new_y;
    model->grid.agents[new_y][new_x] = id;
    model->grid.owner[new_y][new_x] = id;
    if (model->grid.arrival[new_y][new_x] == -1)
        model->grid.arrival[new_y][new_x] = model->bp;
    time_here = 0;
    update_land();
}

void Agent::check_move() {
    bool forest_here {model->grid.veg[y][x] >= FOREST_VAL};
    if (time_here > permanence || !forest_here) { // VEG VALUE
        std::vector<std::pair<int, int>> cells {check_empty_cells()};
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            move(best_cell.first, best_cell.second);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations {check_destinations(leap_distance)};
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                //if (model->grid.suit[best_cell.second][best_cell.first] > model->grid.suit[y][x]) {
                    move(best_cell.first, best_cell.second);
                //}
            } else if (!forest_here) {
                alive = false;
            }
        } else if (!forest_here) {
            alive = false;
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

bool Agent::is_in_grid(int a, int b) {
    if (a >= 0 && a < model->grid.width && b >= 0 && b < model->grid.height)
        return true;
    else
        return false;
}

std::vector<std::pair<int, int>> Agent::check_empty_cells() {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (int i {-1}; i <= 1; ++i) {
        for (int j {-1}; j <= 1; ++j) {
            if (is_in_grid(x+i, y+j)
                //&& model->grid.owner[y+j][x+i] == 0
                && (model->grid.owner[y+j][x+i] == 0 || model->grid.owner[y+j][x+i] == id)
                && model->grid.agents[y+j][x+i] == 0
                && model->grid.elevation[y+j][x+i] >= 1
                && model->grid.suit[y+j][x+i] >= SUIT_VAL // REMOVE THIS LATER!!!
                && model->grid.veg[y+j][x+i] >= FOREST_VAL) // REMOVE THIS LATER!!!
                cells.push_back(std::make_pair(x+i, y+j));
        }
    }
    return cells;
}

// change name of this method
std::vector<std::pair<int, int>> Agent::check_destinations(int distance) {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (int i {-distance}; i <= distance; ++i) {
        for (int j {-distance}; j <= distance; ++j) {
            if (is_in_grid(x+i, y+j)
                && get_distance(x+i, y+j) == distance
                && model->grid.owner[y+j][x+i] == 0
                && model->grid.agents[y+j][x+i] == 0
                && model->grid.elevation[y+j][x+i] >= 1
                && model->grid.suit[y+j][x+i] >= SUIT_VAL // REMOVE THIS LATER!!!
                && model->grid.veg[y+j][x+i] >= FOREST_VAL) // REMOVE THIS LATER!!!
                cells.push_back(std::make_pair(x+i, y+j));
        }
    }
    return cells;
}

int Agent::get_distance(int x_i, int y_i) {
    return round(sqrt(pow(x - x_i, 2) + pow(y - y_i, 2)));
}

int Agent::get_x() {
    return x;
}

int Agent::get_y() {
    return y;
}

bool Agent::is_alive() {
    return alive;
}