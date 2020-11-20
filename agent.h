#ifndef AGENT_H
#define AGENT_H

#include "model.h"

class Model;

class Agent {
    static int new_id;
    int id, population, fission_threshold, k, permanence, time_here,
        leap_distance, total_k;
    double r, x, y;
    bool alive;
    std::vector<std::pair<int, int>> land;
    Model* model;

    public:
        Agent(Model& model, int x, int y, int population,
              int fission_threshold, int k, int permanence, int leap_distance);
        ~Agent();
        void grow();
        Agent* fission();
        void check_fission();
        void step();
        void move(int new_x, int new_y);
        void check_move();
        std::vector<std::pair<int, int>> check_empty_cells(),
                                         check_destinations(int distance);
        std::pair<int, int> get_best_cell(std::vector<std::pair<int, int>> cells);
        int get_distance(int x_i, int y_i);
        bool is_in_grid(int a, int b);
        void update_land();
        int get_x();
        int get_y();
        bool is_alive();
};

#endif