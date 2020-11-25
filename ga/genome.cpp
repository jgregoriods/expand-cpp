#include <iostream> // REMOVE
#include <memory>
#include <random>
#include <vector>

#include "genome.h"
#include "gene.h"

std::random_device seeder1;
std::mt19937 engine1(seeder1());

Genome::Genome() {
    genes.reserve(10);
}

void Genome::mutate() {
    std::uniform_int_distribution<int> dist(0, genes.size() - 1);
    int selected = dist(engine1);
    genes[selected]->mutate();
}

/*
bool Genome::operator< (const Genome &genome) {
    return fitness < genome.fitness;
}
*/

// CHANGE, THIS IS A DUMMY
void Genome::measure_fitness() {
    double sum {};
    for (auto gene: genes)
        sum += gene->get_value();
    fitness = sum;
}

void Genome::add(std::shared_ptr<Gene> gene) {
    genes.push_back(std::move(gene));
}

