#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Algo.h"
#include "Result.h"
#include "RandomSearch.h"
#include "RandomWalk.h"
#include "GreedyCycle.h"
#include <vector>
#include <climits>
#include <vector>
#include "generator.h"
#include <map>
#include <stdexcept>
using std::vector;

enum class SearchType
{
    Greedy,
    Steepest
};

enum class InitialSolutionType
{
    RandomSearch,
    GreedyCycle,
    RandomWalk
};

extern std::map<std::string, InitialSolutionType> InitialSolutionTypeStrings;
extern std::map<std::string, SearchType> SearchTypeStrings;

class LocalSearch : public Algo
{
public:
    SearchType searchType;
    InitialSolutionType initialSolutionType;
    int nPoints;
    int iterations = 0;

    int evaluations = 0;
    LocalSearch(SearchType searchType, InitialSolutionType initialSolutionType, vector<vector<double>> distances, std::default_random_engine rng);
    double calculate_cost(const vector<int> &solution);
    int fixIndex(int index, int solutionSize);
    Result solve();
    void localSearch(vector<int> *solution);
    vector<int> getInitialSolution(InitialSolutionType ist);
    double calculateDelta(vector<int> &solution, vector<int> &move);
    void applyMove(vector<int> *solution, const vector<int> &move);
    generator<vector<int>> neighbourhoodGenerator(vector<int> &currentSolution);
    vector<int> makeIntraMove(int edge1_first, int edge1_second, int edge2_first, int edge2_second);
};

#endif // LOCALSEARCH_H