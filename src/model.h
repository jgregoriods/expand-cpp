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
        Model();
        void setup(int start_date, int init_x, int init_y, int fission_threshold,
                   int k, int permanence, int leap_distance, bool diffuse);
        void run(int n, bool write_files=false, bool show_progress=false);
        void step(bool write_files=false);
        void add(std::shared_ptr<Agent> agent);
        int get_agent(int x, int y);
        int get_owner(int x, int y);
        int get_date(int x, int y);
        double get_vegetation(int x, int y);
        int get_elevation(int x, int y);
        double get_suitability(int x, int y);
        void place_agent(int agent_id, int x, int y);
        void set_owner(int owner_id, int x, int y);
        void record_date(int x, int y);
        int count_agents();
        void update_env();
        void load_dates(std::string path);
        double get_score();
        void write_snapshot();
        void write_asc();
        bool is_in_grid(int x, int y);
        bool is_suitable(int x, int y);
        bool is_forest(int x, int y);
        int get_hg(int x, int y);
        bool diffuse;
    private:
        Grid grid;
        int bp;
        std::vector<std::shared_ptr<Agent>> agents;
        std::vector<std::shared_ptr<Date>> dates;
};

#endif