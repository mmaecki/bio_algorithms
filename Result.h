#ifndef RESULT_H
#define RESULT_H

#include <vector>

struct Result
{
    int bestCost;
    int worstCost;
    int averageCost;
    std::vector<int> bestSolution;
    std::vector<int> worstSolution;

    Result(int bc, int wc, int ac, std::vector<int> bs, std::vector<int> ws)
        : bestCost(bc), worstCost(wc), averageCost(ac), bestSolution(bs), worstSolution(ws) {}
};

#endif // RESULT_H