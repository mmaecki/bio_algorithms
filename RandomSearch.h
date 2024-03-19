#ifndef RANDOMSEARCH_H
#define RANDOMSEARCH_H

#include "Algo.h"
#include <vector>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <random>
using std::iota;
using std::shuffle;
using std::vector;

class RandomSearch : public Algo
{
public:
    double time_limit;
    RandomSearch(vector<vector<int>> distances, double time_limit, std::default_random_engine generator);
    Result solve() override;
};

#endif // RANDOMSEARCH_H