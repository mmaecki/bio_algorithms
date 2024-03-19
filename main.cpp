// #include <vector>
// #include <fstream>
// #include <sstream>
// #include <iostream>
// #include <cmath>
// #include <ctime>
// #include <cstdlib>
// #include <list>
// #include <memory>
// #include <map>
// #include <numeric>
// #include <coroutine>
// #include <algorithm>
// #include <random>
// #include <cassert>
// #include <typeinfo>
// #include <string>
// #include <unistd.h>

// using namespace std;

// auto rng = std::default_random_engine(869468469);

// template <typename T>
// struct generator
// {
//     struct promise_type;
//     using handle_type = std::coroutine_handle<promise_type>;

//     struct promise_type
//     {
//         T value;
//         std::suspend_always initial_suspend() { return {}; }
//         std::suspend_always final_suspend() noexcept { return {}; }
//         generator get_return_object() { return generator{handle_type::from_promise(*this)}; }
//         void unhandled_exception() { std::terminate(); }
//         std::suspend_always yield_value(T val)
//         {
//             value = val;
//             return {};
//         }
//     };

//     bool move_next() { return coro ? (coro.resume(), !coro.done()) : false; }
//     T current_value() { return coro.promise().value; }

//     generator(generator const &) = delete;
//     generator(generator &&other) : coro(other.coro) { other.coro = {}; }
//     ~generator()
//     {
//         if (coro)
//             coro.destroy();
//     }

// private:
//     generator(handle_type h) : coro(h) {}
//     handle_type coro;
// };

// enum SearchType
// {
//     greedy,
//     steepest
// };
// enum InitialSolutionType
// {
//     randomAlg,
//     GC,
//     randomWalk
// };

// enum ProblemInstance
// {
//     TSPA,
//     TSPB,
//     TSPC,
//     TSPD
// };

// std::map<SearchType, std::string> SearchTypeStrings = {
//     {greedy, "greedy"},
//     {steepest, "steepest"}};

// std::map<InitialSolutionType, std::string> InitialSolutionTypeStrings = {
//     {randomAlg, "random"},
//     {GC, "GreedyCycle"},
//     {randomWalk, "randomWalk"}};

// std::map<ProblemInstance, std::string> ProblemInstanceStrings = {
//     {TSPA, "TSPA"},
//     {TSPB, "TSPB"},
//     {TSPC, "TSPC"},
//     {TSPD, "TSPD"}};

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
struct Node
{
    int id;
    int x;
    int y;
};

std::vector<std::vector<int>> calcDistances(const std::vector<Node> &nodes)
{
    std::vector<std::vector<int>> distances(nodes.size(), std::vector<int>(nodes.size()));
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < nodes.size(); j++)
        {
            int x = nodes[i].x - nodes[j].x;
            int y = nodes[i].y - nodes[j].y;
            distances[i][j] = round(sqrt(x * x + y * y));
        }
    }
    return distances;
}

std::vector<std::vector<int>> read_file(std::string filename)
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
                    std::sscanf(line.c_str(), "%d %d %d", &node.id, &node.x, &node.y);
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
    Heuristic,
    LocalSearch
};

std::map<std::string, Method> methodMap = {
    {"random_search", Method::RandomSearch},
    {"random_walk", Method::RandomWalk},
    {"heuristic", Method::Heuristic},
    {"local_search", Method::LocalSearch}};

int main(int argc, char *argv[])
{
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

    std::vector<std::vector<int>> distances = read_file(filename);

    int start_node;
    std::string type;

    std::default_random_engine generator = std::default_random_engine(869468469);

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
        RandomSearch algo(distances, running_time, generator);

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

        RandomWalk algo(distances, running_time, generator);

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
    case Method::Heuristic:

        std::cout << "Heuristic with starting node: " << start_node << std::endl;
        break;
    case Method::LocalSearch:
        if (argc < 5)
        {
            std::cerr << "LocalSearch method requires a type (greedy or steepest).\n";
            return 1;
        }
        type = argv[3];
        std::cout << "LocalSearch with type: " << type << std::endl;
        break;
    }

    delete result;

    return 0;
}