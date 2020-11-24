#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono> //remove
#include <iomanip> //remove
#include <utility>
#include <iterator>
#include <memory>
#include "model.h"
#include "agent.h"
#include "grid.h"
#include "date.h"

using namespace std::chrono; // remove

int main() {
    auto start = high_resolution_clock::now();

    Model model(5000);

    //std::shared_ptr<Agent> agent1(new Agent(model, 229, 76, 76, 76, 26, 16, 15));

    Agent* agent1 = new Agent(model, 229, 76, 76, 76, 26, 16, 15);
    model.add(agent1);
    model.grid.arrival[agent1->get_y()][agent1->get_x()] = model.bp;

    //std::cout << std::fixed << std::setprecision(4);
    model.load_dates("dates");

    model.run(4400);
    model.write_asc();
    std::cout << model.get_score() << std::endl;

    delete agent1;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Runtime: "
              << duration.count() / 1000000.0 << " seconds" << std::endl;

    return 0;
}