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

    Model model(5000);

    Agent* agent1 = new Agent(model, 229, 76, 150, 150, 50, 30, 10);
    model.grid.arrival[agent1->y][agent1->x] = model.bp;

    model.run(4400);
 
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Runtime: "
              << duration.count() / 1000000.0 << " seconds" << std::endl;

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

    std::ofstream file3("arrival.asc");
    file3 << "NCOLS 638" << std::endl;
    file3 << "NROWS 825" << std::endl;
    file3 << "XLLCORNER -2985163.8955" << std::endl;
    file3 << "YLLCORNER -3022031.214" << std::endl;
    file3 << "CELLSIZE 10000" << std::endl;
    file3 << "NODATA_value -1" << std::endl;
    std::ostream_iterator<int> it3(file3, "\t");
    for (int i {0}; i < model.grid.arrival.size(); ++i) {
        std::copy(model.grid.arrival.at(i).begin(),
                  model.grid.arrival.at(i).end(), it3);
        file3 << std::endl;
    }

    return 0;
}