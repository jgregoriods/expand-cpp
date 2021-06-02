#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

#include "model.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

/**
* Implementation of the model class.
*/
Model::Model(int start_date, double maxent) :
    bp(start_date),
    SUIT_VAL(maxent) {
        Grid new_grid(825, 638, start_date);
        grid = new_grid;
        agents.reserve(500000);
}

/**
* Sets up the model by specifying the parameters before run. The initial agent
* is created with the specified parameters and placed in the origin cell, where
* the start date is recorded.
*
* @param coords The (x, y) coordinates of the origin cell.
* @param fission_threshold The maximum population of a village before fissioning.
* @param r The annual growth rate of the village.
* @param k The carrying capacity in persons/100km^2.
* @param permanence The maximum number of years before a village moves.
* @param leap_distance The distance, in km, of leapfrogging.
*/
void Model::setup(std::pair<int, int> coords, int fission_threshold, double r,
                  int k, int permanence, int leap_distance) {
    auto agent = std::make_unique<Agent>(*this, coords.first, coords.second, (double) fission_threshold,
                                         fission_threshold, r, k, permanence, leap_distance);
    add(agent);
    record_date(coords.first, coords.second);
}

/**
* Runs the model for a number of time steps. At the end of the execution, the
* simulated arrival times can be written as an asc file.
*
* @param n The number of time steps (years) to run.
* @param write_files Whether the resulting simulated arrival times should be
*   written as an asc file.
* @param show_progress Whether a progress bar should be shown.
*/
void Model::run(int n, bool write_files, bool show_progress) {
    std::time_t timer1, timer2;
    std::time(&timer1);
    int progress {};
    int k {};
    for (int i {0}; i < n; ++i) {
        step();
        std::time(&timer2);
        if (std::difftime(timer2, timer1) > 180) // stop if > 3 minutes
            break;
        if (show_progress) {
            k++;
            progress = ((double)k / (double)n) * 100;
            std::cout << "\r" << '[' << std::string(progress / 2, '#')
                      << std::string(50 - (progress / 2), ' ')
                      << "] " << progress << "%";
            std::cout.flush();
        }
    }
    if (show_progress)
        std::cout << std::endl;
    if (write_files) {
        write_asc();
    }
}

/**
* The sequence of methods to be executed in a single step of the model.
*/
void Model::step() {
    auto it = agents.begin();
    while (it != agents.end()) {
        auto& agent = *it;
        agent->step();
        ++it;
    }
    bp--;
}

/**
* Adds a newly created agent to the vector of agents.
*
* @param agent A pointer to the agent object.
*/
void Model::add(std::unique_ptr<Agent>& agent) {
    agents.push_back(std::move(agent));
}

/**
* Returns the unique id of the agent occupying a given cell.
*
* @param x The x coordinate of the cell.
* @param y The y coordinate of the cell.
* @return the id of the agent in the cell.
*/
int Model::get_agent(int x, int y) {
    return grid.agents[y][x];
}

/**
* Returns the unique id of the agent who owns a given cell.
*
* @param x The x coordinate of the cell.
* @param y The y coordinate of the cell.
* @return the id of the owner of the cell.
*/
int Model::get_owner(int x, int y) {
    return grid.owner[y][x];
}

/**
* Returns the simulated arrival time in a given cell.
*
* @param x The x coordinate of the cell.
* @param y The y coordinate of the cell.
* @return the simulated arrival time in the cell. This will return -1 if the
*   cell has never been settled.
*/
int Model::get_date(int x, int y) {
    return grid.arrival[y][x];
}

/**
* Returns the elevation of a given cell.
*
* @param x The x coordinate of the cell.
* @param y The y coordinate of the cell.
* @return the elevation of the cell.
*/
int Model::get_elevation(int x, int y) {
    return grid.elevation[y][x];
}

/**
* Returns the suitability of a given cell.
*
* @param x The x coordinate of the cell.
* @param y The y coordinate of the cell.
* @return the suitability of the cell.
*/
double Model::get_suitability(int x, int y) {
    return grid.suitability[y][x];
}

/**
* Adds the unique id of an agent to the respective cell in the agent layer of
* the model's grid.
*
* @param agent_id The id of the agent.
* @param x The x coordinate of the cell.
* @param y The y coordinate of the cell.
*/
void Model::place_agent(int agent_id, int x, int y) {
    grid.agents[y][x] = agent_id;
}

void Model::set_owner(int owner_id, int x, int y) {
    grid.owner[y][x] = owner_id;
}

void Model::record_date(int x, int y) {
    grid.arrival[y][x] = bp;
}

int Model::count_agents() {
    return agents.size();
}

void Model::write_asc() {
    std::ofstream file;
    file.open("output/arrival.asc");
    file << "NCOLS 638" << std::endl;
    file << "NROWS 825" << std::endl;
    file << "XLLCORNER -2985163.8955" << std::endl;
    file << "YLLCORNER -3022031.214" << std::endl;
    file << "CELLSIZE 10000" << std::endl;
    file << "NODATA_value -1" << std::endl;
    std::ostream_iterator<int> it(file, "\t");
    for (int i {0}; i < grid.arrival.size(); ++i) {
        std::copy(grid.arrival.at(i).begin(),
                  grid.arrival.at(i).end(), it);
        file << std::endl;
    }
    file.close();
}

bool Model::is_in_grid(int x, int y) {
    if (x >= 0 && x < grid.width && y >= 0 && y < grid.height)
        return true;
    else
        return false;
}

bool Model::is_suitable(int x, int y) {
    if (is_in_grid(x, y)
        && grid.agents[y][x] == 0
        && grid.suitability[y][x] >= SUIT_VAL)
        return true;
    else
        return false;
}

std::pair<int, int> Model::to_grid(double x, double y) {
    return grid.to_grid(x, y);
}

std::pair<double, double> Model::get_coords(std::string site_name) {
    std::ifstream file("coords/coords.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream split(line);
        std::string name;
        double x, y;
        split >> name;
        if (name == site_name) {
            split >> x >> y;
            return std::make_pair(x, y);
        }
    }
    return std::make_pair(0.0, 0.0);
}
