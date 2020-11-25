#ifndef GA_H
#define GA_H

#include <memory>
#include <vector>

class Genome;

class GA {
    public:
        GA(int population_size, int num_parents, int num_elite, double prob_cross,
           double prob_mut);
        void initialize_population();
        void evolve(int num_generations);
        int population_size;
        int num_parents;
        int num_elite;
        double prob_cross;
        double prob_mut;
        double best_fitness;
        double mean_fitness;
        std::vector<std::shared_ptr<Genome>> population;
};

#endif