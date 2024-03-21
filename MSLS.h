#ifndef MSLS_H
#define MSLS_H

#include <vector>
#include <climits>
#include "LocalSearch.h"
#include "Result.h"

class MSLS : public LocalSearch
{
public:
    double time_limit;
    MSLS(SearchType searchType, InitialSolutionType initialSolutionType, vector<vector<double>> distances, std::default_random_engine rng, double time_limit);
    Result solve();
};

#endif // MSLS_H