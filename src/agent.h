#ifndef AGENT_H
#define AGENT_H

#include <memory>

#include "model.h"

class Model;

class Agent {
    public:
        Agent(Model& model, int x, int y, int population, int fission_threshold,
              int k, int permanence, int leap_distance);
        ~Agent();
        void make_leap_cells(int distance);
        void step();
        void grow();
        void update_land();
        void check_fission();
        std::unique_ptr<Agent> fission();
        void check_move();
        void move(int new_x, int new_y);
        void abandon_land();
        std::vector<std::pair<int, int>> check_empty_cells();
        std::vector<std::pair<int, int>> check_destinations();
        std::vector<std::pair<int, int>> check_leap_cells();
        std::pair<int, int> get_best_cell(std::vector<std::pair<int, int>> cells);
        int get_distance(int x_i, int y_i);
        int get_id();
        int get_x();
        int get_y();
    private:
        static int new_id;
        static std::vector<std::pair<int, int>> leap_cells;
        static std::vector<std::pair<int, int>> ngb;
        int id;
        Model* model;
        int x;
        int y;
        double r;
        int population;
        int fission_threshold;
        int k;
        int total_k;
        int permanence;
        int time_here;
        int leap_distance;
        std::vector<std::pair<int, int>> land;
};

#endif