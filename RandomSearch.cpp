#include "RandomSearch.h"
#include <iostream>

RandomSearch::RandomSearch(vector<vector<int>> distances, double time_limit, std::default_random_engine rng)
    : Algo(distances, "RandomSearch", rng), time_limit(time_limit) {}

Result RandomSearch::solve()
{
    clock_t start, end;
    int solution_size = this->distances.size();
    vector<int> current_solution = vector<int>(solution_size);
    iota(current_solution.begin(), current_solution.end(), 0);
    shuffle(current_solution.begin(), current_solution.end(), this->rng);
    int bestCost = calculate_cost(current_solution);
    vector<int> bestSolution = current_solution;
    double mean = 0.0;
    double M2 = 0.0;
    int iterations = 0;
    start = clock();
    while (true)
    {
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        if (time_taken >= this->time_limit)
        {
            double variance = M2 / (iterations - 1);
            double std_dev = sqrt(variance);
            return Result(bestCost, mean, std_dev, iterations, iterations, bestSolution);
        }
        shuffle(current_solution.begin(), current_solution.end(), this->rng);
        int cost = calculate_cost(current_solution);
        iterations++;
        double delta = cost - mean;
        mean += delta / iterations;
        M2 += delta * (cost - mean);
        if (cost < bestCost)
        {
            bestCost = cost;
            bestSolution = current_solution;
        }
    }
}