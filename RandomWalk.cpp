#include "RandomWalk.h"

RandomWalk::RandomWalk(vector<vector<int>> distances, vector<int> costs, int starting_node, double time_limit)
    : Algo(distances, costs, "RandomWalk", starting_node), time_limit(time_limit) {}

Result RandomWalk::solve()
{
    vector<int> worstSolution;
    int solution_size = this->distances.size();
    vector<int> current_solution = vector<int>(solution_size);
    iota(current_solution.begin(), current_solution.end(), 0);
    shuffle(current_solution.begin(), current_solution.end(), rng);
    int bestCost = calculate_cost(current_solution);
    vector<int> bestSolution = current_solution;
    clock_t start, end;
    start = clock();
    while (true)
    {
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        if (time_taken >= this->time_limit)
        {
            return Result(bestCost, 0, 0, bestSolution, worstSolution);
        }
        int i = rng() % solution_size;
        int j = rng() % solution_size;
        swap(current_solution[i], current_solution[j]);
        int cost = calculate_cost(current_solution);
        if (cost < bestCost)
        {
            bestCost = cost;
            bestSolution = current_solution;
        }
    }
}