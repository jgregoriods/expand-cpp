#ifndef GENOME_H
#define GENOME_H

#include <memory>
#include <vector>

class Gene;

class Genome {
    public:
        Genome();
        //bool operator< (const Genome &genome);
        void mutate();
        void measure_fitness();
        void add(std::shared_ptr<Gene> gene);
        std::vector<std::shared_ptr<Gene>> genes;
        double fitness;
};

#endif