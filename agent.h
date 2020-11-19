#ifndef AGENT_H
#define AGENT_H

#include "model.h"

class Model;

class Agent {
    static int new_id;
    public:
        int id, population, fission_threshold, k, permanence, time_here;
        double r, x, y;
        Model* model;
        Agent(Model& model, int x, int y, int population,
              int fission_threshold, int k, int permanence);
        void grow();
        Agent* fission();
        void check_fission();
        void step();
        void move(int new_x, int new_y);
        void check_move();
        std::vector<std::pair<int, int>> check_empty_cells();
        std::pair<int, int> get_best_cell(std::vector<std::pair<int, int>> cells);
};

#endif