#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <string>
#include <map>
#include <random>
#include "RandomSearch.h"
#include "RandomWalk.h"
#include "GreedyCycle.h"
#include "LocalSearch.h"
#include <iomanip>

struct Node
{
    int id;
    double x;
    double y;
};

std::vector<std::vector<double>> calcDistances(const std::vector<Node> &nodes)
{
    std::vector<std::vector<double>> distances(nodes.size(), std::vector<double>(nodes.size()));
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < nodes.size(); j++)
        {
            double x = nodes[i].x - nodes[j].x;
            double y = nodes[i].y - nodes[j].y;
            distances[i][j] = sqrt(x * x + y * y);
        }
    }
    return distances;
}

std::vector<std::vector<double>> read_file(std::string filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<Node> nodes;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line == "EOF")
            {
                break;
            }

            if (line.find("NODE_COORD_SECTION") != std::string::npos)
            {
                while (std::getline(file, line) && line != "EOF")
                {
                    Node node;
                    // NODE_COORD_SECTION
                    // 1 0.00000e+00 0.00000e+00 2 5.51200e+02 9.96400e+02

                    std::sscanf(line.c_str(), "%d %lf %lf", &node.id, &node.x, &node.y);
                    nodes.push_back(node);
                }
                break;
            }
        }
        file.close();
    }

    return calcDistances(nodes);
}

enum class Method
{
    RandomSearch,
    RandomWalk,
    GreedyCycle,
    LocalSearch
};

std::map<std::string, Method> methodMap = {
    {"random_search", Method::RandomSearch},
    {"random_walk", Method::RandomWalk},
    {"greedy_cycle", Method::GreedyCycle},
    {"local_search", Method::LocalSearch}};

int main(int argc, char *argv[])
{
    std::setprecision(2);

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <method>\n";
        std::cerr << "Methods: random_search, random_walk, heuristic, local_search\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string methodStr = argv[2];

    if (methodMap.find(methodStr) == methodMap.end())
    {
        std::cerr << "Invalid method. Choose from: random_search, random_walk, heuristic, local_search\n";
        return 1;
    }

    Method method = methodMap[methodStr];

    std::vector<std::vector<double>> distances = read_file(filename);

    int start_node;
    std::string type;

    std::default_random_engine rng = std::default_random_engine(869468469);

    Result *result = nullptr;

    switch (method)
    {
    case Method::RandomSearch:
    {
        // read running time
        if (argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " <filename> random_search <running_time>\n";
            std::cerr << "RandomSearch method requires a running time.\n";
            return 1;
        }
        double running_time = std::stod(argv[3]);
        RandomSearch algo(distances, running_time, rng);

        result = new Result(algo.solve());
        std::cout << "Best cost: " << result->bestCost
                  << "\nAverage cost: " << result->averageCost
                  << "\nStandard deviation: " << result->standardDeviation
                  << "\nIterations: " << result->iterations
                  << "\nBest solution: " << std::endl;

        for (int i = 0; i < result->bestSolution.size(); i++)
        {
            std::cout << result->bestSolution[i] << " ";
        }
    }

    break;
    case Method::RandomWalk:
    {
        // read running time
        if (argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " <filename> random_walk <running_time>\n";
            std::cerr << "RandomWalk method requires a running time.\n";
            return 1;
        }
        double running_time = std::stod(argv[3]);

        RandomWalk algo(distances, running_time, rng);

        result = new Result(algo.solve());
        std::cout << "Best cost: " << result->bestCost
                  << "\nAverage cost: " << result->averageCost
                  << "\nStandard deviation: " << result->standardDeviation
                  << "\nIterations: " << result->iterations
                  << "\nEvaluations: " << result->evaluations
                  << "\nBest solution: " << std::endl;

        for (int i = 0; i < result->bestSolution.size(); i++)
        {
            std::cout << result->bestSolution[i] << " ";
        }
    }
    break;
    case Method::GreedyCycle:

    {
        if (argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " <filename> greedy_cycle <starting_node>\n";
            std::cerr << "GreedyCycle method requires a starting node.\n";
            return 1;
        }
        start_node = std::stoi(argv[3]);
        GreedyCycle algo(distances, start_node, rng);

        result = new Result(algo.solve());
        std::cout << "Best cost: " << result->bestCost
                  << "\nBest solution: " << std::endl;

        for (int i = 0; i < result->bestSolution.size(); i++)
        {
            std::cout << result->bestSolution[i] << " ";
        }
    }
    break;
    case Method::LocalSearch:
    {
        if (argc < 5)
        {
            std::cerr << "Usage: " << argv[0] << " <filename> local_search <initial_solution_type> <local_search_type>\n";
            std::cerr << "LocalSearch method requires an initial solution type and a local search type.\n";
            std::cerr << "Initial solution types: random_search, greedy_cycle, random_walk\n";
            std::cerr << "Local search types: greedy, steepest\n";
            return 1;
        }
        std::string initialSolutionTypeStr = argv[3];
        std::string localSearchTypeStr = argv[4];

        if (InitialSolutionTypeStrings.find(initialSolutionTypeStr) == InitialSolutionTypeStrings.end())
        {
            std::cerr << "Invalid initial solution type. Choose from: random_search, greedy_cycle, random_walk\n";
            return 1;
        }

        if (SearchTypeStrings.find(localSearchTypeStr) == SearchTypeStrings.end())
        {
            std::cerr << "Invalid local search type. Choose from: greedy, steepest\n";
            return 1;
        }

        InitialSolutionType initialSolutionType = InitialSolutionTypeStrings[initialSolutionTypeStr];
        SearchType localSearchType = SearchTypeStrings[localSearchTypeStr];

        LocalSearch algo(localSearchType, initialSolutionType, distances, rng);

        result = new Result(algo.solve());
        std::cout << "Best cost: " << result->bestCost
                  << "\nAverage cost: " << result->averageCost
                  << "\nStandard deviation: " << result->standardDeviation
                  << "\nIterations: " << result->iterations
                  << "\nEvaluations: " << result->evaluations
                  << "\nBest solution: " << std::endl;

        for (int i = 0; i < result->bestSolution.size(); i++)
        {
            std::cout << result->bestSolution[i] << " ";
        }
    }
    break;
    }

    delete result;

    return 0;
}