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
    vector<vector<double>> distances;
    string name;
    vector<std::pair<double, double>> partialResults;
    Algo(std::vector<std::vector<double>> distances, std::string name, std::default_random_engine rng);
    std::default_random_engine rng;
    virtual Result solve() = 0;
    double calculate_cost(vector<int> solution);
    string get_name();
};

#endif // ALGO_H