#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>

#include "agent.h"
#include "date.h"
#include "grid.h"

class Agent;
class Date;
class Grid;

class Model {
    public:
        Model(std::string culture, int start_date, double maxent, double forest,
              std::string date_folder);
        void setup(std::pair<int, int> coords, int fission_threshold, int k,
                   int permanence, int leap_distance);
        void run(int n, bool write_files=false, bool show_progress=false);
        void step(bool write_files=false);
        void add(std::unique_ptr<Agent>& agent);
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
        void load_dates();
        std::pair<double, int> get_score();
        void write_snapshot();
        void write_asc();
        void write_dates();
        bool is_in_grid(int x, int y);
        bool is_suitable(int x, int y);
        bool is_forest(int x, int y);
        std::pair<int, int> to_grid(double x, double y);
        std::pair<double, double> get_coords(std::string site_name);
    private:
        double SUIT_VAL;
        double FOREST_VAL;
        std::string culture;
        std::string date_folder;
        Grid grid;
        int bp;
        std::vector<std::unique_ptr<Agent>> agents;
        std::vector<std::unique_ptr<Date>> dates;
};

#endif