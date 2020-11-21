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

    Agent* agent1 = new Agent(model, 229, 76, 100, 100, 50, 10, 30);
    model.grid.arrival[agent1->get_y()][agent1->get_x()] = model.bp;

    model.run(4400);
    model.write_asc();
 
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Runtime: "
              << duration.count() / 1000000.0 << " seconds" << std::endl;

    return 0;
}