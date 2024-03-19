#ifndef RESULT_H
#define RESULT_H

#include <vector>

struct Result
{
    int bestCost;
    double averageCost;
    double standardDeviation;
    int iterations;
    std::vector<int> bestSolution;

    Result(int best_cost, double average_cost, double standard_deviation, int iterations, std::vector<int> bs)
        : bestCost(best_cost), averageCost(average_cost), standardDeviation(standard_deviation), iterations(iterations), bestSolution(bs) {}
};

#endif // RESULT_H