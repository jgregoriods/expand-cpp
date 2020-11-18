#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "agent.h"
#include "grid.h"

class Agent;

class Model {
    public:
        std::vector<Agent*> agents;
        Grid grid;
        Model();
        void step();
};

#endif