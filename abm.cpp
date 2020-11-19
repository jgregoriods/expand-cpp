#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono> //remove
#include <utility>
#include <iterator>

#include "model.h"
#include "agent.h"
#include "grid.h"

using namespace std::chrono; // remove

int main() {
    auto start = high_resolution_clock::now();

    Model model(4500);
    Agent agent1(model, 200, 200, 150, 150, 40, 20, 0);
    model.grid.arrival[agent1.y][agent1.x] = model.bp;

    for (int i {0}; i < 4500; ++i) {
        model.step();
    }
    
    std::ofstream file("owners.asc");
    file << "NCOLS 638" << std::endl;
    file << "NROWS 825" << std::endl;
    file << "XLLCORNER -2985163.8955" << std::endl;
    file << "YLLCORNER -3022031.214" << std::endl;
    file << "CELLSIZE 10000" << std::endl;
    file << "NODATA_value 0" << std::endl;
    std::ostream_iterator<int> it(file, "\t");
    for (int i {0}; i < model.grid.owner.size(); ++i) {
        std::copy(model.grid.owner.at(i).begin(),
                  model.grid.owner.at(i).end(), it);
        file << std::endl;
    }

    std::ofstream file2("agents.asc");
    file2 << "NCOLS 638" << std::endl;
    file2 << "NROWS 825" << std::endl;
    file2 << "XLLCORNER -2985163.8955" << std::endl;
    file2 << "YLLCORNER -3022031.214" << std::endl;
    file2 << "CELLSIZE 10000" << std::endl;
    file2 << "NODATA_value 0" << std::endl;
    std::ostream_iterator<int> it2(file2, "\t");
    for (int i {0}; i < model.grid.agents.size(); ++i) {
        std::copy(model.grid.agents.at(i).begin(),
                  model.grid.agents.at(i).end(), it2);
        file2 << std::endl;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken by function: "
              << duration.count() << " microseconds" << std::endl;

    return 0;
}