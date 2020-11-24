#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>

#include "agent.h"
#include "date.h"
#include "grid.h"

class Agent;
class Date;

class Model {
    public:
        int bp;
        std::vector<Agent*> agents;
        //std::vector<std::shared_ptr<Agent>> agents;
        std::vector<Date*> dates;
        Grid grid;
        Model(int start_date);
        void load_dates(std::string path);
        void update_env();
        void step(bool write=false);
        void run(int n, bool write=false);
        void write_snapshot();
        void write_asc();
        double get_score();
        void add(Agent* agent);
};

#endif