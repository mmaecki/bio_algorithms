#include "MSLS.h"

MSLS::MSLS(SearchType searchType, InitialSolutionType initialSolutionType, vector<vector<double>> distances, std::default_random_engine rng, double time_limit)
    : LocalSearch(searchType, initialSolutionType, distances, rng), time_limit(time_limit)
{
}

Result MSLS::solve()
{

    double mean = 0.0;
    double M2 = 0.0;
    double start = clock();
    vector<int> solution = getInitialSolution(this->initialSolutionType);
    double initialCost = calculate_cost(solution);
    double bestCost = initialCost;
    double worstCost = initialCost;
    vector<int> worstSolution = solution;
    vector<int> bestSolution = solution;
    int number_of_restarts = 0;
    while (true)
    {
        double time_taken = double(clock() - start) / double(CLOCKS_PER_SEC);
        if (time_taken > this->time_limit)
        {
            double variance = M2 / (this->iterations - 1);
            double std_dev = sqrt(variance);
            return Result(bestCost, mean, worstCost, std_dev, this->iterations, this->evaluations, bestSolution, worstSolution, time_taken, this->partialResults, initialCost);
        }
        localSearch(&solution);
        double currentCost = calculate_cost(solution);
        this->partialResults.push_back(std::make_pair(time_taken, currentCost));
        this->iterations++;
        this->evaluations++;
        double delta = currentCost - mean;
        mean += delta / this->iterations;
        M2 += delta * (currentCost - mean);
        if (currentCost > worstCost)
        {
            worstCost = currentCost;
            worstSolution = solution;
        }
        if (currentCost < bestCost)
        {
            bestCost = currentCost;
            bestSolution = solution;
        }
        number_of_restarts++;
        solution = getInitialSolution(this->initialSolutionType);
    }
}