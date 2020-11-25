#include <cmath>
#include <iostream> // remove
#include <memory>
#include <utility>
#include <vector>

#include "agent.h"
#include "model.h"

int Agent::new_id {1};
std::vector<std::pair<int, int>> Agent::mask {};
std::vector<std::pair<int, int>> ngb {std::make_pair(-1, -1),
                                      std::make_pair(0, -1),
                                      std::make_pair(1, -1),
                                      std::make_pair(-1, 0),
                                      std::make_pair(1, 0),
                                      std::make_pair(-1, 1),
                                      std::make_pair(0, 1),
                                      std::make_pair(1, 1)};
const double SUIT_VAL {0.15};
const double FOREST_VAL {0.15};

Agent::Agent(Model& model, int x, int y, int population,
             int fission_threshold, int k, int permanence, int leap_distance) :
    id(new_id++), model(&model), x(x), y(y), r(0.025), population(population),
    k(k), fission_threshold(fission_threshold), permanence(permanence),
    time_here(0), leap_distance(leap_distance), total_k(k), alive(true) {
        if (model.grid.agents[y][x] == 0 && model.grid.owner[y][x] == 0)
            model.grid.agents[y][x] = id;
            model.grid.owner[y][x] = id;
        // model.agents.push_back(this); /////////////////////////////////////////////////////////////////////////
        land.reserve(9);
        mask.reserve(900);
        if (leap_distance > 0 && mask.size() < 1)
            make_mask(leap_distance);
}

Agent::~Agent() {}

void Agent::abandon_land() {
    model->grid.agents[y][x] = 0;
    model->grid.owner[y][x] = 0;
    for (auto cell: land)
        model->grid.owner[cell.second][cell.first] = 0;
}

void Agent::grow() {
    population += round(population * r);
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

std::shared_ptr<Agent> Agent::fission() {
//Agent* Agent::fission() {
    population /= 2;
    /*
    Agent* new_agent = new Agent(*model, x, y, population,
                                 fission_threshold, k, permanence,
                                 leap_distance); // leak here REALLLLLLLLLLL
    
    */
    return std::make_shared<Agent>(*model, x, y, population,
                                   fission_threshold, k, permanence,
                                   leap_distance);
}

void Agent::check_fission() {
    if (population > fission_threshold) {
        std::vector<std::pair<int, int>> cells {check_destinations(1)};
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            //Agent* new_agent = fission();
            std::shared_ptr<Agent> new_agent = fission();
            new_agent->move(best_cell.first, best_cell.second);
            model->add(new_agent);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations {check_leap_cells()};
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                //Agent* new_agent = fission();
                std::shared_ptr<Agent> new_agent = fission();
                new_agent->move(best_cell.first, best_cell.second);
                model->add(new_agent);
            }
        }
    }
}

void Agent::move(int new_x, int new_y) {
    if (model->grid.agents[y][x] == id) {
        model->grid.agents[y][x] = 0;
        model->grid.owner[y][x] = 0;
        for (auto cell: land)
            model->grid.owner[cell.second][cell.first] = 0;
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
            std::vector<std::pair<int, int>> destinations {check_leap_cells()};
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                    move(best_cell.first, best_cell.second);
            } else if (!forest_here && model->agents.size() > 1) {
                alive = false;
            }
        } else if (!forest_here && model->agents.size() > 1) {
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
    for (auto cell: cells) {
        double val = model->grid.suit[cell.second][cell.first]; 
        if (val > best_val) {
            best_val = val;
            best_cell = cell;
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
    for (auto cell: ngb) {
        int i {cell.first}, j {cell.second};
        if (is_in_grid(x+i, y+j)
            && (model->grid.owner[y+j][x+i] == 0 || model->grid.owner[y+j][x+i] == id)
            && model->grid.agents[y+j][x+i] == 0
            && model->grid.elevation[y+j][x+i] >= 1
            && model->grid.suit[y+j][x+i] >= SUIT_VAL // REMOVE THIS LATER!!!
            && model->grid.veg[y+j][x+i] >= FOREST_VAL) // REMOVE THIS LATER!!!
            cells.push_back(std::make_pair(x+i, y+j));
    }
    //}
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

std::vector<std::pair<int, int>> Agent::check_leap_cells() {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (auto cell: mask) {
        int i {cell.first}, j {cell.second};
        if (is_in_grid(x+i, y+j)
            && model->grid.owner[y+j][x+i] == 0
            && model->grid.agents[y+j][x+i] == 0
            && model->grid.elevation[y+j][x+i] >= 1
            && model->grid.suit[y+j][x+i] >= SUIT_VAL // REMOVE THIS LATER!!!
            && model->grid.veg[y+j][x+i] >= FOREST_VAL) // REMOVE THIS LATER!!!
            cells.push_back(std::make_pair(x+i, y+j));
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

void Agent::make_mask(int radius) {
    for (int i {-radius}; i <= radius; ++i) {
        for (int j {-radius}; j <= radius; ++j) {
            if (get_distance(x + i, y + j) == radius)
                mask.push_back(std::make_pair(i, j));
        }
    }
}