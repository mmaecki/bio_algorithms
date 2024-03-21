#ifndef GREEDYCYCLE_H
#define GREEDYCYCLE_H

#include "Algo.h"
#include <vector>
#include <climits>

using std::vector;

class GreedyCycle : public Algo
{
public:
    int starting_node;
    GreedyCycle(vector<vector<double>> distances, int starting_node, std::default_random_engine rng);
    Result solve() override;
};

#endif // GREEDYCYCLE_H