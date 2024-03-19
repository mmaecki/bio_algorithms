#ifndef ALGO_H
#define ALGO_H

#include <vector>
#include <string>
#include "Result.h"
#include <random>

using std::string;
using std::vector;

class Algo
{
public:
    vector<vector<int>> distances;
    string name;
    Algo(std::vector<std::vector<int>> distances, std::string name, std::default_random_engine generator);
    std::default_random_engine generator;
    virtual Result solve() = 0;
    int calculate_cost(vector<int> solution);
    string get_name();
};

#endif // ALGO_H