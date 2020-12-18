#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "agent.h"
#include "model.h"

// unique id for each village
int Agent::new_id {1};

// vector with (x, y) differences to each leap cell
std::vector<std::pair<int, int>> Agent::leap_cells {};

// vector with (x, y) difference to the 8 immediate neighbors
std::vector<std::pair<int, int>> Agent::neighbors {std::make_pair(-1, -1),
                                                   std::make_pair(0, -1),
                                                   std::make_pair(1, -1),
                                                   std::make_pair(-1, 0),
                                                   std::make_pair(1, 0),
                                                   std::make_pair(-1, 1),
                                                   std::make_pair(0, 1),
                                                   std::make_pair(1, 1)};

std::vector<std::pair<int, int>> Agent::move_cells = {};

Agent::Agent(Model& model, int x, int y, int population, int fission_threshold,
             int k, int permanence, int leap_distance) :
    id {new_id++},
    model {&model},
    x {x},
    y {y},
    r {0.025}, // growth rate is kept constant (2.5%) in all models
    population {population},
    fission_threshold {fission_threshold},
    k {k},
    total_k {k},
    permanence {permanence},
    time_here {0},
    leap_distance {leap_distance} {
        land.reserve(9);
        // only take ownership immediately if this is the first agent in cell
        // (not from fission)
        if (model.get_agent(x, y) == 0 && model.get_owner(x, y) == 0) {
            model.place_agent(id, x, y);
            model.set_owner(id, x, y);
        }
        leap_cells.reserve(900);
        // calculate the (x, y) difference to each leap cell in case this is
        // the first agent created.
        if (leap_distance > 0 && leap_cells.size() < 1)
            make_leap_cells(leap_distance);
        
        if (move_cells.size() < 1) {
            for (int i {-2}; i <= 2; ++i)
                for (int j {-2}; j <= 2; ++j)
                    if (get_distance(x + i, y + j) == 2)
                        move_cells.push_back(std::make_pair(i, j));
        }
}

Agent::~Agent() {}

/*
* Populates a vector with the (x, y) difference to each cell at leap
* distance. This is to avoid having to calculate it every time a search is
* performed.
*/
void Agent::make_leap_cells(int distance) {
    for (int i {-distance}; i <= distance; ++i) {
        for (int j {-distance}; j <= distance; ++j) {
            if (get_distance(x + i, y + j) == distance)
                leap_cells.push_back(std::make_pair(i, j));
        }
    }
}

/*
* Sequene of methods to be executed every tick (year) of the model.
*/
void Agent::step() {
    grow();
    check_fission();
    check_move();
}

/*
* Population grows exponentially. Update land is called to check whether
* max density/carrying capactiy has been reached.
*/
void Agent::grow() {
    population += round(population * r);
    update_land();
}

/*
* Checks whether population is above max density. If so, tries to add a new
* cell to the village's land. If that is not possible, population is stabilized
* at current max density.
*/
void Agent::update_land() {
    while (population > total_k) {
        std::vector<std::pair<int, int>> cells {check_destinations(neighbors)};
        if (cells.size() == 0)
            cells = check_destinations(move_cells);
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            land.push_back(best_cell);
            model->set_owner(id, best_cell.first, best_cell.second);
            // record the date in the new cell in case it has never been
            // settled or owned.
            if (model->get_date(best_cell.first, best_cell.second) == -1)
                model->record_date(best_cell.first, best_cell.second);
            total_k += k;
        } else {
            population = total_k;
        }
    }
}

/*
* Checks whether population is above fission threshold. If yes and there are
* cells available to move, the village fissions and the new village moves to
* the best empty cell. In case there are no cells in the immediate
* neighborhood and the village can leap, a new search is performed with the
* leap distance.
*/
void Agent::check_fission() {
    if (population > fission_threshold) {
        std::vector<std::pair<int, int>> cells = check_destinations(neighbors);
        if (cells.size() == 0)
            cells = check_destinations(move_cells);
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            std::unique_ptr<Agent> new_agent = fission();
            new_agent->move(best_cell.first, best_cell.second);
            model->add(new_agent);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations = check_leap_cells();
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                std::unique_ptr<Agent> new_agent = fission();
                new_agent->move(best_cell.first, best_cell.second);
                model->add(new_agent);
            }
        }
    }
}

/*
* Splits the population in half and creates a new village with the same
* parameters.
*/
std::unique_ptr<Agent> Agent::fission() {
    population /= 2;
    auto agent = std::make_unique<Agent>(*model, x, y, population, fission_threshold,
                                         k, permanence, leap_distance);
    return std::move(agent);
}

/*
* Checks whether the village has been in the same cell for too long or whether
* the % forest in the cell is below the minimum threshold. If yes and there
* are emtpy cells in the neighborhood, the village moves. Otherwise, if the
* village can leap, a new search is performed with the leap distance.
*/
void Agent::check_move() {
    bool forest_here {model->is_forest(x, y)};
    if (time_here > permanence || !forest_here) {
        std::vector<std::pair<int, int>> cells = check_empty_cells(neighbors);
        if (cells.size() == 0)
            cells = check_empty_cells(move_cells);
        if (cells.size() > 0) {
            std::pair<int, int> best_cell = get_best_cell(cells);
            move(best_cell.first, best_cell.second);
        } else if (leap_distance > 0) {
            std::vector<std::pair<int, int>> destinations = check_leap_cells();
            if (destinations.size() > 0) {
                std::pair<int, int> best_cell = get_best_cell(destinations);
                    move(best_cell.first, best_cell.second);
            }
        }
    }
    ++time_here;
}

/*
* Moves the agent to a new cell, takes ownership of it and records the
* date in case it was never settled or owned.
*/
void Agent::move(int new_x, int new_y) {
    // release ownership of land
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

/*
* Resets the agent of current cell and the owner of all cells in village's
* land to 0.
*/
void Agent::abandon_land() {
    model->place_agent(0, x, y);
    model->set_owner(0, x, y);
    for (auto cell: land)
        model->set_owner(0, cell.first, cell.second);
}

/*
* Returns a vector of (x, y) coordinates of cells which are not settled,
* not owned and are inhabitable (based on suitability and vegetation layers).
*/
std::vector<std::pair<int, int>> Agent::check_empty_cells(std::vector<std::pair<int, int>> dist) {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (auto cell: dist) {
        int i {cell.first}, j {cell.second};
        if (model->is_suitable(x+i, y+j)
            && (model->get_owner(x+i, y+j) == 0
                || model->get_owner(x+i, y+j) == id))
            cells.push_back(std::make_pair(x+i, y+j));
    }
    return cells;
}

/*
* Returns a vector of (x, y) coordinates of cells in the immediate
* neighborhood which are not owned and are inhabitable.
*/
std::vector<std::pair<int, int>> Agent::check_destinations(std::vector<std::pair<int, int>> dist) {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(9);
    for (auto cell: dist) {
        int i {cell.first}, j {cell.second};
        if (model->is_suitable(x+i, y+j) && model->get_owner(x+i, y+j) == 0)
            cells.push_back(std::make_pair(x+i, y+j));
    }
    return cells;
}

/*
* Returns a vector of (x, y) coordinates of cells at leap distance which
* are not settled, not owned and are inhabitable.
*/
std::vector<std::pair<int, int>> Agent::check_leap_cells() {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(900);
    for (auto cell: leap_cells) {
        int i {cell.first}, j {cell.second};
        if (model->is_suitable(x+i, y+j) && model->get_owner(x+i, y+j) == 0)
            cells.push_back(std::make_pair(x+i, y+j));
    }
    return cells;
}

/*
* Given a vector of (x, y) cell coordinates, returns the one with the highest
* suitability value. 
*/
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

/*
* Returns the distance (in grid cells) from the current cell to another cell
* given its coordinates.
*/
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