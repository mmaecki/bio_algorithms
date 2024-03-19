#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include "Algo.h"
#include <vector>
#include <algorithm>
#include <ctime>

using std::iota;
using std::shuffle;
using std::swap;
using std::vector;

class RandomWalk : public Algo
{
public:
    double time_limit;
    RandomWalk(vector<vector<int>> distances, vector<int> costs, int starting_node, double time_limit);
    Result solve() override;
};

#endif // RANDOMWALK_H