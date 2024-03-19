#include "RandomSearch.h"

RandomSearch::RandomSearch(vector<vector<int>> distances, double time_limit, std::default_random_engine generator)
    : Algo(distances, "RandomSearch", generator), time_limit(time_limit) {}

Result RandomSearch::solve()
{
    clock_t start, end;
    start = clock();
    int solution_size = this->distances.size();
    vector<int> current_solution = vector<int>(solution_size);
    iota(current_solution.begin(), current_solution.end(), 0);
    shuffle(current_solution.begin(), current_solution.end(), this->generator);
    int bestCost = calculate_cost(current_solution);
    vector<int> bestSolution = current_solution;
    while (true)
    {
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        if (time_taken >= this->time_limit)
        {
            return Result(bestCost, 0, 0, bestSolution, {});
        }
        shuffle(current_solution.begin(), current_solution.end(), this->generator);
        int cost = calculate_cost(current_solution);
        if (cost < bestCost)
        {
            bestCost = cost;
            bestSolution = current_solution;
        }
    }
}