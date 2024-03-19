#include "LocalSearch.h"

LocalSearch::LocalSearch(SearchType searchType, InitialSolutionType initialSolutionType, vector<vector<int>> distances, vector<int> costs, int starting_node)
    : Algo(distances, costs, "LS", starting_node), searchType(searchType), initialSolutionType(initialSolutionType)
{
    this->name += "_" + SearchTypeStrings[searchType];
    this->name += "_" + InitialSolutionTypeStrings[initialSolutionType];
    nPoints = distances.size();
} // ... constructor and other methods ...

int LocalSearch::calculate_cost(const vector<int> &solution)
{
    int cost = 0;
    for (int j = 0; j < solution.size() - 1; j++)
    {
        cost += this->distances[solution[j]][solution[j + 1]];
    }
    cost += this->distances[solution[solution.size() - 1]][solution[0]];
    for (int j = 0; j < solution.size(); j++)
    {
        cost += this->costs[solution[j]];
    }
    return cost;
}

int LocalSearch::fixIndex(int index, int solutionSize)
{
    if (index < 0)
    {
        return solutionSize + index;
    }
    if (index >= solutionSize)
    {
        return index - solutionSize;
    }
    return index;
}

Result LocalSearch::solve()
{
    vector<int> solution = getInitialSolution(this->initialSolutionType);
    // cout << "Initial solution created" << endl;
    // cout << solution.size() << endl;
    localSearch(&solution);
    return Result(calculate_cost(solution), 0, 0, solution, vector<int>());
}

void LocalSearch::localSearch(vector<int> *solution)
{
    while (true)
    {
        auto neighbourhoodIterator2 = neighbourhoodGenerator(*solution);
        int bestDelta = INT32_MAX;
        vector<int> bestMove;
        while (neighbourhoodIterator2.move_next())
        {
            vector<int> move = neighbourhoodIterator2.current_value();
            int delta = calculateDelta(*solution, move);
            if (delta < bestDelta)
            {
                bestDelta = delta;
                bestMove = move;
                if (searchType == greedy && bestDelta < 0)
                {
                    break;
                }
            }
        }
        if (bestDelta >= 0)
        {
            // cout << "Local search finished" << endl;
            break;
        }
        // cout << "Applying move" << endl;
        // cout << bestMove[0] << " " << bestMove[1] << " " << bestMove[2] << " " << bestMove[3] << endl;
        applyMove(solution, bestMove);
    }
}

vector<int> LocalSearch::getInitialSolution(InitialSolutionType ist)
{
    if (ist == randomAlg)
    {
        RandomSearch rs = RandomSearch(distances, costs, starting_node, 0.0);
        return rs.solve().bestSolution;
    }
    else if (ist == randomWalk)
    {
        RandomWalk rw = RandomWalk(distances, costs, starting_node, 0.0);
        return rw.solve().bestSolution;
    }
    else if (ist == GC)
    {
        GreedyCycle gc = GreedyCycle(distances, costs, starting_node);
        return gc.solve().bestSolution;
    }
}

int LocalSearch::calculateDelta(vector<int> &solution, vector<int> &move)
{
    int delta;
    if (move.size() == 4)
    {
        // edge exchnge
        int edge1_first = solution[move[0]];
        int edge1_second = solution[move[1]];
        int edge2_first = solution[move[2]];
        int edge2_second = solution[move[3]];
        int oldCost = distances[edge1_first][edge1_second] + distances[edge2_first][edge2_second];
        int newCost = distances[edge1_first][edge2_first] + distances[edge1_second][edge2_second];
        delta = newCost - oldCost;
    }
    else
    {
        throw runtime_error("Wrong size of best move");
    }
    return delta;
}

void LocalSearch::applyMove(vector<int> *solution, const vector<int> &move)
{
    int j = move[1];
    int k = move[2];
    reverse(solution->begin() + j, solution->begin() + k + 1);
}

generator<vector<int>> LocalSearch::neighbourhoodGenerator(vector<int> &currentSolution)
{
    vector<int> temp_vec = {0, 0, 0, 0};
    vector<int> move = vector<int>(temp_vec);
    vector<int> indexes = currentSolution;
    shuffle(indexes.begin(), indexes.end(), rng);

    for (int x = 0; x < indexes.size(); x++)
    {
        int i = indexes[x];
        int node1 = currentSolution[i];
        int node1_next = currentSolution[fixIndex(i + 1, currentSolution.size())];
        for (int j = i + 2; j < currentSolution.size(); j++)
        {
            int node2 = currentSolution[j];
            int node2_next = currentSolution[fixIndex(j + 1, currentSolution.size())];
            co_yield makeIntraMove(i, i + 1, j, fixIndex(j + 1, currentSolution.size()));
        }
    }
}

vector<int> LocalSearch::makeIntraMove(int edge1_first, int edge1_second, int edge2_first, int edge2_second)
{
    return {edge1_first, edge1_second, edge2_first, edge2_second};
}