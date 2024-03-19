#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Algo.h"
#include "Result.h"
#include "RandomSearch.h"
#include "RandomWalk.h"
#include "GreedyCycle.h"
#include <vector>
#include <climits>

using std::vector;

enum SearchType
{
    greedy,
    steepest
};
enum InitialSolutionType
{
    randomAlg,
    randomWalk,
    GC
};

class LocalSearch : public Algo
{
public:
    SearchType searchType;
    InitialSolutionType initialSolutionType;
    int nPoints;

    LocalSearch(SearchType searchType, InitialSolutionType initialSolutionType, vector<vector<int>> distances, vector<int> costs, int starting_node);
    int calculate_cost(const vector<int> &solution);
    int fixIndex(int index, int solutionSize);
    Result solve();
    void localSearch(vector<int> *solution);
    vector<int> getInitialSolution(InitialSolutionType ist);
    int calculateDelta(vector<int> &solution, vector<int> &move);
    void applyMove(vector<int> *solution, const vector<int> &move);
    generator<vector<int>> neighbourhoodGenerator(vector<int> &currentSolution);
    vector<int> makeIntraMove(int edge1_first, int edge1_second, int edge2_first, int edge2_second);
};

#endif // LOCALSEARCH_H