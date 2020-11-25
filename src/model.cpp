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

void Model::load_dates(std::string path) {
    for (const auto& entry: recursive_directory_iterator(path)) {
        //Date* date = new Date(entry.path());
        std::shared_ptr<Date> date = std::make_shared<Date>(entry.path());
        dates.push_back(date);
    }
}

void Model::update_env() {
    if (bp % 50 == 0) {
        grid.veg.clear();
        std::string filename {"layers/veg" + std::to_string(bp) + ".asc"};
        grid.veg = grid.add_layer(filename);
    }
}

void Model::step(bool write) {
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
    if (write)
        write_snapshot();
}

void Model::run(int n, bool write) {
    int progress {};
    int k {};
    for (int i {0}; i < n; ++i) {
        step(write);
        k++;
        progress = ((double)k / (double)n) * 100;
        std::cout << "\r" << '[' << std::string(progress / 2, '#')
                  << std::string(50 - (progress / 2), ' ')
                  << "] " << progress << "%";
        std::cout.flush();
    }
    std::cout << std::endl;
}

void Model::write_snapshot() {
    std::string filename {"snapshots/" + std::to_string(bp) + ".csv"};
    std::ofstream file;
    file.open(filename);
    for (auto agent: agents)
        file << agent->get_x() << ", " << agent->get_y() << "\n";
    file.close();
}

void Model::write_asc() {
    std::ofstream file("output/owners.asc");
    file << "NCOLS 638" << std::endl;
    file << "NROWS 825" << std::endl;
    file << "XLLCORNER -2985163.8955" << std::endl;
    file << "YLLCORNER -3022031.214" << std::endl;
    file << "CELLSIZE 10000" << std::endl;
    file << "NODATA_value 0" << std::endl;
    std::ostream_iterator<int> it(file, "\t");
    for (int i {0}; i < grid.owner.size(); ++i) {
        std::copy(grid.owner.at(i).begin(),
                  grid.owner.at(i).end(), it);
        file << std::endl;
    }

    std::ofstream file2("output/agents.asc");
    file2 << "NCOLS 638" << std::endl;
    file2 << "NROWS 825" << std::endl;
    file2 << "XLLCORNER -2985163.8955" << std::endl;
    file2 << "YLLCORNER -3022031.214" << std::endl;
    file2 << "CELLSIZE 10000" << std::endl;
    file2 << "NODATA_value 0" << std::endl;
    std::ostream_iterator<int> it2(file2, "\t");
    for (int i {0}; i < grid.agents.size(); ++i) {
        std::copy(grid.agents.at(i).begin(),
                  grid.agents.at(i).end(), it2);
        file2 << std::endl;
    }

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

double Model::get_score() {
    double total {};
    for (auto date: dates) {
        std::pair<int, int> cell {grid.to_grid(date->x, date->y)};
        int sim_bp {grid.arrival[cell.second][cell.first]};
        if (date->probs.find(sim_bp) != date->probs.end())
            total += date->probs[sim_bp]; // REAL LEAAAAKKKK
    }
    return total / dates.size();
}

//void Model::add(Agent* agent) {
void Model::add(std::shared_ptr<Agent> agent) {
    agents.push_back(std::move(agent));
}