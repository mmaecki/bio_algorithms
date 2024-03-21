#include "LocalSearch.h"

std::map<std::string, SearchType> SearchTypeStrings = {
    {"greedy", SearchType::Greedy},
    {"steepest", SearchType::Steepest}};

std::map<std::string, InitialSolutionType> InitialSolutionTypeStrings = {
    {"random_search", InitialSolutionType::RandomSearch},
    {"greedy_cycle", InitialSolutionType::GreedyCycle},
    {"random_walk", InitialSolutionType::RandomWalk}};
LocalSearch::LocalSearch(SearchType searchType, InitialSolutionType initialSolutionType, vector<vector<double>> distances, std::default_random_engine rng)
    : Algo(distances, "LS", rng), searchType(searchType), initialSolutionType(initialSolutionType)
{
    nPoints = distances.size();
}

double LocalSearch::calculate_cost(const vector<int> &solution)
{
    double cost = 0;
    for (int j = 0; j < solution.size() - 1; j++)
    {
        cost += this->distances[solution[j]][solution[j + 1]];
    }
    cost += this->distances[solution[solution.size() - 1]][solution[0]];

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
    double start = clock();
    vector<int> solution = getInitialSolution(this->initialSolutionType);
    double initialCost = calculate_cost(solution);
    // copy solution to worstSolution
    vector<int> worstSolution = solution;
    localSearch(&solution);
    double end = clock();
    double best_cost = calculate_cost(solution);
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    this->partialResults.push_back(std::make_pair(best_cost, time_taken));

    return Result(best_cost, best_cost, initialCost, 0.0, this->iterations, this->evaluations, solution, worstSolution, time_taken, this->partialResults, initialCost);
}

void LocalSearch::localSearch(vector<int> *solution)
{
    while (true)
    {
        auto neighbourhoodIterator2 = neighbourhoodGenerator(*solution);
        double bestDelta = std::numeric_limits<double>::max();
        vector<int> bestMove;
        while (neighbourhoodIterator2.move_next())
        {
            vector<int> move = neighbourhoodIterator2.current_value();
            double delta = calculateDelta(*solution, move);

            if (delta < bestDelta)
            {

                bestDelta = delta;
                bestMove = move;
                if (searchType == SearchType::Greedy && bestDelta < 0)
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
        this->iterations++;

        applyMove(solution, bestMove);
    }
}

vector<int> LocalSearch::getInitialSolution(InitialSolutionType ist)
{
    if (ist == InitialSolutionType::RandomSearch)
    {
        RandomSearch rs = RandomSearch(distances, 0.0, rng);
        return rs.solve().bestSolution;
    }
    else if (ist == InitialSolutionType::RandomWalk)
    {
        RandomWalk rw = RandomWalk(distances, 0.0, rng);
        return rw.solve().bestSolution;
    }
    else if (ist == InitialSolutionType::GreedyCycle)
    {
        int starting_node = rng() % nPoints;
        GreedyCycle gc = GreedyCycle(distances, starting_node, rng);
        return gc.solve().bestSolution;
    }
}

double LocalSearch::calculateDelta(vector<int> &solution, vector<int> &move)
{
    int delta;
    if (move.size() == 4)
    {
        // edge exchnge
        int edge1_first = solution[move[0]];
        int edge1_second = solution[move[1]];
        int edge2_first = solution[move[2]];
        int edge2_second = solution[move[3]];
        double oldCost = distances[edge1_first][edge1_second] + distances[edge2_first][edge2_second];
        double newCost = distances[edge1_first][edge2_first] + distances[edge1_second][edge2_second];
        delta = newCost - oldCost;
        this->evaluations++;
    }
    else
    {
        throw std::runtime_error("Wrong size of best move");
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