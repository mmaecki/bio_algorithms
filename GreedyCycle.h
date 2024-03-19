#ifndef GREEDYCYCLE_H
#define GREEDYCYCLE_H

#include "Algo.h"
#include <vector>
#include <climits>

using std::vector;

class GreedyCycle : public Algo
{
public:
    GreedyCycle(vector<vector<int>> distances, vector<int> costs, int starting_node);
    Result solve() override;
};

#endif // GREEDYCYCLE_H