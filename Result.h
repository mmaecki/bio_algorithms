#ifndef RESULT_H
#define RESULT_H

#include <vector>

struct Result
{
    double bestCost;
    double averageCost;
    double worstCost;
    double standardDeviation;
    int iterations;
    int evaluations;
    double runningTime;
    std::vector<std::pair<double, double>> partialResults;
    std::vector<int> bestSolution;
    std::vector<int> worstSolution;
    double initialSolutionCost;

    Result(double best_cost, double average_cost, double worst_cost, double standard_deviation, int iterations, int evaluations, std::vector<int> bs, std::vector<int> ws, double running_time, std::vector<std::pair<double, double>> partial_results, double initial_solution_cost)
        : bestCost(best_cost), averageCost(average_cost), worstCost(worst_cost), standardDeviation(standard_deviation), iterations(iterations), evaluations(evaluations), bestSolution(bs), worstSolution(ws), runningTime(running_time), partialResults(partial_results), initialSolutionCost(initial_solution_cost) {}
};

#endif // RESULT_H