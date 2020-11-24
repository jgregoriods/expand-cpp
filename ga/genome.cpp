#include <vector>

class Gene;

class Genome {
    public:
        std::vector<Gene*> genes;
        double fitness;
        Genome();
        void mutate();
}