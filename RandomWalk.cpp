#include "RandomWalk.h"
#include <iostream>
RandomWalk::RandomWalk(vector<vector<double>> distances, double time_limit, std::default_random_engine rng)
    : Algo(distances, "RandomWalk", rng), time_limit(time_limit)
{
    this->nPoints = distances.size();
}

Result RandomWalk::solve()
{
    vector<int> worstSolution;
    int solution_size = this->distances.size();
    vector<int> current_solution = vector<int>(solution_size);
    iota(current_solution.begin(), current_solution.end(), 0);
    shuffle(current_solution.begin(), current_solution.end(), rng);
    double bestCost = calculate_cost(current_solution);
    double currentCost = bestCost;
    vector<int> bestSolution = current_solution;
    int iterations = 0;
    clock_t start, end;
    double mean = 0.0;
    double M2 = 0.0;
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
        int i = this->rng() % solution_size;
        int j = ((this->rng() % (solution_size - 1)) + 1 + i) % solution_size;

        currentCost += calculate_delta(current_solution, i, j);
        swap(current_solution[i], current_solution[j]);
        iterations++;
        double delta = currentCost - mean;
        mean += delta / iterations;
        M2 += delta * (currentCost - mean);
        if (currentCost < bestCost)
        {
            bestCost = currentCost;
            bestSolution = current_solution;
        }
    }
}

double RandomWalk::calculate_delta(vector<int> &solution, int i, int j)
{
    int prev_i = (i - 1 + this->nPoints) % this->nPoints;
    int next_i = (i + 1) % this->nPoints;
    int prev_j = (j - 1 + this->nPoints) % this->nPoints;
    int next_j = (j + 1) % this->nPoints;

    double oldCost, newCost;

    if ((i + 1) % this->nPoints == j)
    { // j follows i
        oldCost = distances[solution[prev_i]][solution[i]] + distances[solution[i]][solution[j]] + distances[solution[j]][solution[next_j]];
        newCost = distances[solution[prev_i]][solution[j]] + distances[solution[j]][solution[i]] + distances[solution[i]][solution[next_j]];
    }
    else if ((j + 1) % this->nPoints == i)
    { // i follows j
        oldCost = distances[solution[prev_j]][solution[j]] + distances[solution[j]][solution[i]] + distances[solution[i]][solution[next_i]];
        newCost = distances[solution[prev_j]][solution[i]] + distances[solution[i]][solution[j]] + distances[solution[j]][solution[next_i]];
    }
    else
    { // i and j are not neighbors
        oldCost = distances[solution[prev_i]][solution[i]] + distances[solution[i]][solution[next_i]] + distances[solution[prev_j]][solution[j]] + distances[solution[j]][solution[next_j]];
        newCost = distances[solution[prev_i]][solution[j]] + distances[solution[j]][solution[next_i]] + distances[solution[prev_j]][solution[i]] + distances[solution[i]][solution[next_j]];
    }

    this->evaluations++;
    return newCost - oldCost;
}