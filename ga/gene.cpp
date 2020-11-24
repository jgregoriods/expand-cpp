#include <vector>
#include <random>
#include <iostream>
#include "gene.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(0.0, 1.0);

Gene::Gene(int value, int min, int max, int step) :
    value(value),
    min(min),
    max(max),
    step(step) {}

void Gene::mutate() {
    
}

int main() {
    Gene gene(1,2,3,4);
    gene.mutate();
}