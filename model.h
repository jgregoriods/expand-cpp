#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "agent.h"
#include "grid.h"
//#include "date.h"

class Agent;
//class Date;

class Model {
    public:
        int bp;
        std::vector<Agent*> agents;
        //std::vector<Date*> dates;
        Grid grid;
        Model(int start_date);
        //void load_dates();
        void update_env();
        void step(bool write=false);
        void run(int n, bool write=false);
        void write_snapshot();
        void write_asc();
};

#endif