#ifndef GENE_H
#define GENE_H

class Gene {
    public:
        int value, min, max, step;
        Gene(int value, int min, int max, int step);
        void mutate();
};

#endif