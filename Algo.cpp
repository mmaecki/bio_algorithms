#include <random>
#include "Algo.h"

Algo::Algo(std::vector<std::vector<double>> distances, std::string name, std::default_random_engine rng)
    : distances(distances), name(name), rng(rng) {}

double Algo::calculate_cost(vector<int> solution)
{
    double cost = 0;
    for (int j = 0; j < solution.size() - 1; j++)
    {
        cost += distances[solution[j]][solution[j + 1]];
    }
    cost += distances[solution[solution.size() - 1]][solution[0]];

    return cost;
}

string Algo::get_name()
{
    return this->name;
}