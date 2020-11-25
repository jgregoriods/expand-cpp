#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "ga.h"
#include "gene.h"
#include "genome.h"

std::random_device seeder2;
std::mt19937 engine2(seeder2());

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

void GA::initialize_population() {}

int main() {
       Gene gene(-1,50,100);
       std::cout << gene.get_value() << std::endl;
       return 0;
}