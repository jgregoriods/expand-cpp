#ifndef GENOME_H
#define GENOME_H

#include <memory>
#include <vector>

class Gene;

class Genome {
    public:
        Genome();
        void mutate();
        void add(std::shared_ptr<Gene> gene);
        std::vector<std::shared_ptr<Gene>> genes;
        double fitness;
};

#endif