#include "GreedyCycle.h"

GreedyCycle::GreedyCycle(vector<vector<int>> distances, vector<int> costs, int starting_node)
    : Algo(distances, costs, "GreedyCycle", starting_node) {}

Result GreedyCycle::solve()
{
    vector<int> worstSolution;
    int solution_size = this->distances.size();
    vector<int> current_solution;
    vector<bool> visited(this->costs.size());
    current_solution.push_back(this->starting_node);
    visited[this->starting_node] = true;

    while (current_solution.size() < solution_size)
    {
        int smallest_increase = INT32_MAX;
        int insert_index = -1;
        int insert_node = -1;

        for (int j = 0; j < current_solution.size(); j++)
        { // For each node in the cycle
            int min_distance = INT32_MAX;
            int min_index = -1;
            for (int k = 0; k < this->distances.size(); k++)
            { // find the closest unvisited node
                if (visited[k])
                    continue;
                int curr = -this->distances[current_solution[j == 0 ? current_solution.size() - 1 : j - 1]][current_solution[j]] + this->distances[current_solution[j == 0 ? current_solution.size() - 1 : j - 1]][k] + this->distances[k][current_solution[j]] + this->costs[k];
                if (curr < min_distance)
                {
                    min_distance = curr;
                    min_index = k;
                }
            }
            if (min_distance < smallest_increase)
            {
                smallest_increase = min_distance;
                insert_index = j;
                insert_node = min_index;
            }
        } // end
        current_solution.insert(current_solution.begin() + insert_index, insert_node);
        visited[insert_node] = true;
    }
    return Result(0, 0, 0, current_solution, worstSolution);
}