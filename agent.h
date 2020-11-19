#ifndef AGENT_H
#define AGENT_H

#include "model.h"

class Model;

class Agent {
    static int new_id;
    public:
        int id;
        int population;
        int fission_threshold;
        int k;
        double r;
        double x;
        double y;
        Model* model;
        Agent(Model& model, int x, int y, int population,
              int fission_threshold, int k);
        void grow();
        Agent* fission();
        void check_fission();
        void step();
        void move(int new_x, int new_y);
        std::vector<std::pair<int, int>> check_empty_cells();
};

#endif