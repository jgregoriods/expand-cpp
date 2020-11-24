#include <chrono> //remove
#include <fstream>
#include <iomanip> //remove
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "agent.h"
#include "date.h"
#include "grid.h"
#include "model.h"

using namespace std::chrono; // remove

int main() {
    auto start = high_resolution_clock::now();

    Model model(5000);

    //std::shared_ptr<Agent> agent1(new Agent(model, 229, 76, 76, 76, 26, 16, 15));

    Agent* agent1 = new Agent(model, 229, 76, 76, 76, 26, 16, 0);
    model.add(agent1);
    model.grid.arrival[agent1->get_y()][agent1->get_x()] = model.bp;

    std::cout << std::fixed << std::setprecision(4);
    model.load_dates("dates");

    model.run(1000);
    model.write_asc();
    std::cout << model.get_score() << std::endl;

    /*
    * The correct way to do this would be with smart pointers.
    * For now, let's follow the old-fashioned way and manually delete the pointers.
    * Change later.
    */
    for (auto agent: model.agents)
        delete agent;
    for (auto date: model.dates)
        delete date;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Runtime: "
              << duration.count() / 1000000.0 << " seconds" << std::endl;

    return 0;
}