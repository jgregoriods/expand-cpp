#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream> // remove
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

#include "model.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

Model::Model(int start_date) : bp(start_date) {
    Grid new_grid(825, 638);
    grid = new_grid;
    agents.reserve(500000);
    dates.reserve(100);
}

void Model::run(int n, bool write_files, bool show_progress) {
    std::time_t timer1, timer2;
    std::time(&timer1);
    int progress {};
    int k {};
    for (int i {0}; i < n; ++i) {
        step(write_files);
        std::time(&timer2);
        if (std::difftime(timer2, timer1) > 180) // STOP IF > 3 MINUTES...
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
    if (write_files)
        write_asc();
}

void Model::step(bool write_files) {
    auto it = agents.begin();
    while (it != agents.end()) {
        //Agent* agent = *it;
        auto agent = *it;
        if (!agent->is_alive()) {
            //delete agent;
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

void Model::add(std::shared_ptr<Agent> agent) {
    agents.push_back(std::move(agent));
}

void Model::update_env() {
    if (bp % 50 == 0) {
        grid.veg.clear();
        std::string filename {"layers/veg" + std::to_string(bp) + ".asc"};
        grid.veg = grid.add_layer(filename);
    }
}

void Model::load_dates(std::string path) {
    for (const auto& entry: recursive_directory_iterator(path)) {
        std::shared_ptr<Date> date = std::make_shared<Date>(entry.path());
        dates.push_back(date);
    }
}

double Model::get_score() {
    double total {};
    for (auto date: dates) {
        std::pair<int, int> cell {grid.to_grid(date->get_x(), date->get_y())};
        int sim_bp {grid.arrival[cell.second][cell.first]};
        total += date->get_prob(sim_bp);
    }
    return total / dates.size();
}

void Model::write_snapshot() {
    std::string filename {"python/snapshots/" + std::to_string(bp) + ".csv"};
    std::ofstream file;
    file.open(filename);
    for (auto agent: agents)
        file << agent->get_x() << ", " << agent->get_y() << "\n";
    file.close();
}

void Model::write_asc() {
    std::ofstream file3("output/arrival.asc");
    file3 << "NCOLS 638" << std::endl;
    file3 << "NROWS 825" << std::endl;
    file3 << "XLLCORNER -2985163.8955" << std::endl;
    file3 << "YLLCORNER -3022031.214" << std::endl;
    file3 << "CELLSIZE 10000" << std::endl;
    file3 << "NODATA_value -1" << std::endl;
    std::ostream_iterator<int> it3(file3, "\t");
    for (int i {0}; i < grid.arrival.size(); ++i) {
        std::copy(grid.arrival.at(i).begin(),
                  grid.arrival.at(i).end(), it3);
        file3 << std::endl;
    }
}

int Model::get_bp() {
    return bp;
}

int Model::get_n_agents() {
    return agents.size();
}