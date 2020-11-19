#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "agent.h"
#include "grid.h"

class Agent;

class Model {
    public:
        int bp;
        std::vector<Agent*> agents;
        Grid grid;
        Model(int start_date);
        void step();
};

#endif