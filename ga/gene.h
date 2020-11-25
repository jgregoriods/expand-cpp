#ifndef GENE_H
#define GENE_H

#include <memory>
#include <random>

class Gene {
    public:
        Gene(int value, int lower, int upper);
        void mutate();
        int get_value();
    private:
        int value;
        int lower;
        int upper;
};

#endif