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
        Model(int start_date);
        void run(int n, bool write_files=false);
        void step(bool write_files=false);
        void add(std::shared_ptr<Agent> agent);
        void update_env();
        void load_dates(std::string path);
        double get_score();
        void write_snapshot();
        void write_asc();
        int get_bp();
        int get_n_agents();
        Grid grid;
    private:
        int bp;
        std::vector<std::shared_ptr<Agent>> agents;
        std::vector<std::shared_ptr<Date>> dates;
};

#endif