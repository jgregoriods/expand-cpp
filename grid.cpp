#include <vector>
#include "grid.h"

Grid::Grid() {
    std::vector<std::vector<int>> v(825, std::vector<int> (638, 0));
    this->agents = v;
}