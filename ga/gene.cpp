#include <cmath>
#include <memory>
#include <random>
#include <vector>

#include "gene.h"

std::random_device seeder;
std::mt19937 engine(seeder());

Gene::Gene(int value, int lower, int upper) :
    value(value),
    lower(lower),
    upper(upper) {
        if (value == -1) {
            std::uniform_int_distribution<int> dist(lower, upper);
            this->value = dist(engine);
        }
    }

void Gene::mutate() {
    std::normal_distribution<double> dist(0.0, upper - lower);
    int new_val {};
    do {
        new_val = value + round(dist(engine));
    } while (new_val < lower || new_val > upper);
    value = new_val;
}

int Gene::get_value() {
    return value;
}