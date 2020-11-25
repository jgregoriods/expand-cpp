#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "ga.h"
#include "gene.h"
#include "genome.h"

std::random_device seeder2;
std::mt19937 engine2(seeder2());

bool sort_genomes(const std::shared_ptr<Genome>& g1, const std::shared_ptr<Genome>& g2) {
       return g1->fitness > g2->fitness;
}

GA::GA(int population_size, int num_parents, int num_elite, double prob_cross,
       double prob_mut) :
       population_size(population_size),
       num_parents(num_parents),
       num_elite(num_elite),
       prob_cross(prob_cross),
       prob_mut(prob_mut),
       best_fitness(0.0),
       mean_fitness(0.0) {
              population.reserve(population_size);
       }

void GA::initialize_population() {
       for (int i {0}; i < population_size; ++i) {
              std::shared_ptr<Genome> genome = std::make_shared<Genome>();

              // Change the way genes are init, -1 just looks ugly, put it at the end
              // as default param
              genome->add(std::make_shared<Gene>(-1, 50, 250)); // fission threshold
              genome->add(std::make_shared<Gene>(-1, 20, 100)); // k
              genome->add(std::make_shared<Gene>(-1, 10, 30)); // permanence
              genome->add(std::make_shared<Gene>(-1, 10, 30)); // leap distance

              population.push_back(std::move(genome));
       }
}

void GA::evolve(int num_generations) {
       for (int i {0}; i < num_generations; ++i) {
              for (auto genome: population)
                     genome->measure_fitness();
              std::sort(population.begin(), population.end(), sort_genomes);
       }
}

int main() {
       GA ga(500, 200, 50, 0.8, 0.2);
       ga.initialize_population();
       ga.evolve(1);
       return 0;
}