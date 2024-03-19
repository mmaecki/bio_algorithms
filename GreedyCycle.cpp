#include "GreedyCycle.h"

GreedyCycle::GreedyCycle(vector<vector<double>> distances, int starting_node, std::default_random_engine rng)
    : Algo(distances, "GreedyCycle", rng), starting_node(starting_node) {}

Result GreedyCycle::solve()
{
    vector<int> worstSolution;
    int solution_size = this->distances.size();
    vector<int> current_solution;
    vector<bool> visited(this->distances.size());
    current_solution.push_back(this->starting_node);
    visited[this->starting_node] = true;

    while (current_solution.size() < solution_size)
    {

        double smallest_increase = std::numeric_limits<double>::max();
        int insert_index = -1;
        int insert_node = -1;

        for (int j = 0; j < current_solution.size(); j++)
        { // For each node in the cycle
            double min_distance = std::numeric_limits<double>::max();
            int min_index = -1;
            for (int k = 0; k < this->distances.size(); k++)
            { // find the closest unvisited node
                if (visited[k])
                    continue;
                double curr = -this->distances[current_solution[j == 0 ? current_solution.size() - 1 : j - 1]][current_solution[j]] + this->distances[current_solution[j == 0 ? current_solution.size() - 1 : j - 1]][k] + this->distances[k][current_solution[j]];
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
    double current_cost = calculate_cost(current_solution);
    return Result(current_cost, current_cost, 0, 1, 1, current_solution);
}