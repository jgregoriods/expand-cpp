#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono> //remove

#include "model.h"
#include "agent.h"
#include "grid.h"

using namespace std::chrono; // remove

int main() {
    auto start = high_resolution_clock::now();
    /*
    Agent agent(50);
    Model model;
    model.agents.push_back(&agent);
    model.agents[0]->grow();
    std::cout << model.agents[0]->population << std::endl;
    std::cout << agent.population << std::endl;
    */

    /*
    Model model("jonas");
    Agent agent(model, 50, 3.3, 4.4);
    std::cout << agent.coord.x << "\t" << agent.coord.y << std::endl;
    std::cout << agent.model->name << std::endl;
    agent.model->name = "darcy";
    std::cout << model.name << std::endl;
    */

   /*
    Model model;
    Agent agent1(model, 4.5, 6.7, 150, 200);

    for (int i {0}; i < 1000; ++i) {
        model.agents[0]->step();
    }

    std::cout << model.agents.size() << std::endl;
    */

    /*
    std::string line;
    std::vector<std::vector<double>> ele;
    std::ifstream file("ele.asc");
    
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            ele.push_back(std::vector<double>());
            std::stringstream split(line);
            double value;
            while (split >> value)
                ele.back().push_back(value);
        }
    }
    
    file.close();
    */
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> dist(10, 600);

    Model model;
    Agent agent1(model, dist(engine), dist(engine), 120, 150);
    Agent agent2(model, dist(engine), dist(engine), 120, 150);
    Agent agent3(model, dist(engine), dist(engine), 120, 150);
    Agent agent4(model, dist(engine), dist(engine), 120, 150);
    Agent agent5(model, dist(engine), dist(engine), 120, 150);
    Agent agent6(model, dist(engine), dist(engine), 120, 150);

    for (int i {0}; i < 1200; ++i) {
        model.step();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken by function: "
              << duration.count() << " microseconds" << std::endl; 

    std::cout << model.agents.size() << std::endl;

    return 0;
}