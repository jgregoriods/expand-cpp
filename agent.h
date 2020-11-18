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
        double x;
        double y;
        Model* model;
        Agent(Model& model, int x, int y, int population,
              int fission_threshold);
        void grow();
        void fission();
        void step();
        void move(int new_x, int new_y);
};

#endif