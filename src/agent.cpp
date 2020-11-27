#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "agent.h"
#include "model.h"

int Agent::new_id {1};
std::vector<std::pair<int, int>> Agent::mask {};
std::vector<std::pair<int, int>> Agent::ngb {std::make_pair(-1, -1),
                                             std::make_pair(0, -1),
                                             std::make_pair(1, -1),
                                             std::make_pair(-1, 0),
                                             std::make_pair(1, 0),
                                             std::make_pair(-1, 1),
                                             std::make_pair(0, 1),
                                             std::make_pair(1, 1)};
const double SUIT_VAL {0.5};
const double FOREST_VAL {0.5};

Agent::Agent(Model& model, int x, int y, int population, int fission_threshold,
             int k, int permanence, int leap_distance) :
    id(new_id++),
    model(&model),
    x(x),
    y(y),
    r(0.025),
    population(population),
    fission_threshold(fission_threshold),
    k(k),
    total_k(k),
    permanence(permanence),
    time_here(0),
    leap_distance(leap_distance),
    alive(true) {
        if (model.get_agent(x, y) == 0 && model.get_owner(x, y) == 0) {
            model.place_agent(id, x, y);
            model.set_owner(id, x, y);
        }
        land.reserve(9);
        mask.reserve(900);
        if (leap_distance > 0 && mask.size() < 1)
            make_mask(leap_distance);
}

Agent::~Agent() {}

void Agent::make_mask(int radius) {
    for (int i {-radius}; i <= radius; ++i) {
        for (int j {-radius}; j <= radius; ++j) {
            if (get_distance(x + i, y + j) == radius)
                mask.push_back(std::make_pair(i, j));
        }
    }
}

void Agent::step() {
    grow();
    check_fission();
    check_move();
    time_here++;
}

void Agent::grow() {
    population += round(population * r);
    update_land();
}

void Agent::update_land() {
    while (population > total_k) {
        std::vector<std::pair<int, int>> cells {check_destinations()};
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            land.push_back(best_cell);
            model->set_owner(id, best_cell.first, best_cell.second);
            if (model->get_date(best_cell.first, best_cell.second) == -1)
                model->record_date(best_cell.first, best_cell.second);
            total_k += k;
        } else {
            population = total_k;
        }
    }
}

void Agent::check_fission() {
    if (population > fission_threshold) {
        std::vector<std::pair<int, int>> cells {check_destinations()};
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            std::shared_ptr<Agent> new_agent = fission();
            new_agent->move(best_cell.first, best_cell.second);
            model->add(new_agent);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations {check_leap_cells()};
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                std::shared_ptr<Agent> new_agent = fission();
                new_agent->move(best_cell.first, best_cell.second);
                model->add(new_agent);
            }
        }
    }
}

std::shared_ptr<Agent> Agent::fission() {
    population /= 2;
    return std::make_shared<Agent>(*model, x, y, population, fission_threshold,
                                   k, permanence, leap_distance);
}

void Agent::check_move() {
    bool forest_here {model->get_vegetation(x, y) >= FOREST_VAL};
    if (time_here > permanence || !forest_here) {
        std::vector<std::pair<int, int>> cells {check_empty_cells()};
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            move(best_cell.first, best_cell.second);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations {check_leap_cells()};
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                    move(best_cell.first, best_cell.second);
            } else if (!forest_here && model->count_agents() > 1) {
                alive = false;
            }
        } else if (!forest_here && model->count_agents() > 1) {
            alive = false;
        }
    }
}

void Agent::move(int new_x, int new_y) {
    if (model->get_agent(x, y) == id) {
        abandon_land();
        land.clear();
        total_k = k;
    }
    x = new_x;
    y = new_y;
    model->place_agent(id, new_x, new_y);
    model->set_owner(id, new_x, new_y);
    if (model->get_date(new_x, new_y) == -1)
        model->record_date(new_x, new_y);
    time_here = 0;
    update_land();
}

void Agent::abandon_land() {
    model->place_agent(0, x, y);
    model->set_owner(0, x, y);
    for (auto cell: land)
        model->set_owner(0, cell.first, cell.second);
}

std::vector<std::pair<int, int>> Agent::check_empty_cells() {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (auto cell: ngb) {
        int i {cell.first}, j {cell.second};
        if (is_suitable(x+i, y+j)
            && (model->get_owner(x+i, y+j) == 0
                || model->get_owner(x+i, y+j) == id))
            cells.push_back(std::make_pair(x+i, y+j));
    }
    return cells;
}

std::vector<std::pair<int, int>> Agent::check_destinations() {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (auto cell: ngb) {
        int i {cell.first}, j {cell.second};
        if (is_suitable(x+i, y+j) && model->get_owner(x+i, y+j) == 0)
            cells.push_back(std::make_pair(x+i, y+j));
    }
    return cells;
}

std::vector<std::pair<int, int>> Agent::check_leap_cells() {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (auto cell: mask) {
        int i {cell.first}, j {cell.second};
        if (is_suitable(x+i, y+j) && model->get_owner(x+i, y+j) == 0)
            cells.push_back(std::make_pair(x+i, y+j));
    }
    return cells;
}

bool Agent::is_suitable(int cell_x, int cell_y, bool own) {
    if (model->is_in_grid(cell_x, cell_y)
        && model->get_agent(cell_x, cell_y) == 0
        && model->get_elevation(cell_x, cell_y) >= 1
        && model->get_suitability(cell_x, cell_y) >= SUIT_VAL
        && model->get_vegetation(cell_x, cell_y) >= FOREST_VAL)
        return true;
    else
        return false;
}

std::pair<int, int> Agent::get_best_cell(std::vector<std::pair<int, int>> cells) {
    double best_val {-1};
    std::pair<int, int> best_cell;
    for (auto cell: cells) {
        double val = model->get_suitability(cell.first, cell.second); 
        if (val > best_val) {
            best_val = val;
            best_cell = cell;
        }
    }
    return best_cell;
}

int Agent::get_distance(int x_i, int y_i) {
    return round(sqrt(pow(x - x_i, 2) + pow(y - y_i, 2)));
}

int Agent::get_id() {
    return id;
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