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

Model::Model(std::string culture, int start_date, double maxent, double forest) :
    culture(culture),
    bp(start_date),
    SUIT_VAL(maxent),
    FOREST_VAL(forest) {
        Grid new_grid(825, 638, culture, start_date);
        grid = new_grid;
        agents.reserve(500000);
        dates.reserve(100);
}

void Model::setup(std::pair<int, int> coords, int fission_threshold,
                  int k, int permanence, int leap_distance) {
    auto agent = std::make_unique<Agent>(*this, coords.first, coords.second, fission_threshold,
                                         fission_threshold, k, permanence, leap_distance);
    add(agent);
    record_date(coords.first, coords.second);
}

void Model::run(int n, bool write_files, bool show_progress) {
    std::time_t timer1, timer2;
    std::time(&timer1);
    int progress {};
    int k {};
    for (int i {0}; i < n; ++i) {
        step(write_files);
        std::time(&timer2);
        if (std::difftime(timer2, timer1) > 60) // STOP IF > 3 MINUTES...
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
    load_dates();
    std::cout << std::fixed << std::setprecision(4) << get_score() << std::endl;
    if (write_files)
        write_asc();
        write_dates();
}

void Model::step(bool write_files) {
    auto it = agents.begin();
    while (it != agents.end()) {
        auto& agent = *it;
        if (!agent->is_alive()) {
            agent->abandon_land();
            agent.reset();
            it = agents.erase(it);
        } else {
            agent->step();
            ++it;
        }
    }
    bp--;
    update_env();
    if (write_files)
        write_snapshot();
}

void Model::add(std::unique_ptr<Agent>& agent) {
    agents.push_back(std::move(agent));
}

int Model::get_agent(int x, int y) {
    return grid.agents[y][x];
}

int Model::get_owner(int x, int y) {
    return grid.owner[y][x];
}

int Model::get_date(int x, int y) {
    return grid.arrival[y][x];
}

double Model::get_vegetation(int x, int y) {
    return grid.vegetation[y][x];
}

int Model::get_elevation(int x, int y) {
    return grid.elevation[y][x];
}

double Model::get_suitability(int x, int y) {
    return grid.suitability[y][x];
}

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

void Model::update_env() {
    if (bp % 100 == 0) {
        std::string filename {"layers/veg/veg" + std::to_string(bp) + ".asc"};
        grid.vegetation = grid.layer_from_file(filename);
    }
}

void Model::load_dates() {
    std::string path {"dates/" + culture};
    for (const auto& entry: recursive_directory_iterator(path)) {
        std::unique_ptr<Date> date = std::make_unique<Date>(entry.path());
        dates.push_back(std::move(date));
    }
}

double Model::get_score() {
    double total {};
    for (auto& date: dates) {
        std::pair<int, int> cell {grid.to_grid(date->get_x(), date->get_y())};
        int x {cell.first}, y {cell.second};
        std::vector<int> sim_dates;
        for (int i {-5}; i <= 5; ++i) {
            for (int j {-5}; j <= 5; ++j) {
                int sim_bp {grid.arrival[y+j][x+i]};
                if (sim_bp > -1)
                    sim_dates.push_back(sim_bp);
            }
        }
        int date_sum {};
        for (int sim_date: sim_dates)
            date_sum += sim_date;
        int avg_sim_bp = date_sum / sim_dates.size();
        date->set_prob(avg_sim_bp);
        date->year = avg_sim_bp; // REMOVE /////////////////////////////////////////////////////////////////////////////////
        total += date->get_prob();
    }
    return total / dates.size();
}

void Model::write_snapshot() {
    std::string filename {"python/snapshots/" + std::to_string(bp) + ".csv"};
    std::ofstream file;
    file.open(filename);
    for (auto& agent: agents)
        file << agent->get_x() << ", " << agent->get_y() <<  ", " << agent->breed << "\n";
    file.close();
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

void Model::write_dates() {
    std::ofstream file;
    file.open("output/dates.csv");
    file << "x,y,score\n";
    for (auto& date: dates)
        file << date->get_x() << ", " << date->get_y() << ", " << date->get_prob() << ", " << date->year << "\n";
    file.close();
}

bool Model::is_in_grid(int x, int y) {
    if (x >= 0 && x < grid.width && y >= 0 && y < grid.height)
        return true;
    else
        return false;
}

bool Model::is_forest(int x, int y) {
    return grid.vegetation[y][x] >= FOREST_VAL;
}

bool Model::is_suitable(int x, int y) {
    if (is_in_grid(x, y)
        && grid.agents[y][x] == 0
        && grid.suitability[y][x] >= SUIT_VAL
        && grid.vegetation[y][x] >= FOREST_VAL)
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