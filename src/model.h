#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>

#include "agent.h"
#include "grid.h"

class Agent;
class Grid;

class Model {
    public:
        Model(int start_date, double maxent);
        void setup(std::pair<int, int> coords, int fission_threshold, double r, int k,
                   int permanence, int leap_distance);
        void run(int n, bool write_files=false, bool show_progress=false);
        void step();
        void add(std::unique_ptr<Agent>& agent);
        int get_agent(int x, int y);
        int get_owner(int x, int y);
        int get_date(int x, int y);
        int get_elevation(int x, int y);
        double get_suitability(int x, int y);
        void place_agent(int agent_id, int x, int y);
        void set_owner(int owner_id, int x, int y);
        void record_date(int x, int y);
        int count_agents();
        void write_asc();
        bool is_in_grid(int x, int y);
        bool is_suitable(int x, int y);
        std::pair<int, int> to_grid(double x, double y);
        std::pair<double, double> get_coords(std::string site_name);
    private:
        double SUIT_VAL;
        Grid grid;
        int bp;
        std::vector<std::unique_ptr<Agent>> agents;
};

#endif
