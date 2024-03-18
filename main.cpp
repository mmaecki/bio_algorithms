#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <list>
#include <memory>
#include <map>
#include <numeric>
#include <coroutine>
#include <algorithm>
#include <random>
#include <cassert>
#include <typeinfo>
#include <string>
#include<unistd.h> 

using namespace std;

auto rng = std::default_random_engine(869468469);
struct Result{
    int bestCost;
    int worstCost;
    int averageCost;
    vector<int> bestSolution;
    vector<int> worstSolution;

    Result(int bc, int wc, int ac, vector<int> bs, vector<int> ws)
        : bestCost(bc), worstCost(wc), averageCost(ac), bestSolution(bs), worstSolution(ws) {}
};


class Algo {
public:
    vector<vector<int>> distances;
    vector<int> costs;
    string name;
    int starting_node;
    Algo(vector<vector<int>> distances, vector<int> costs, string name, int starting_node)
        : distances(distances), costs(costs), name(name), starting_node(starting_node) {}
    virtual Result solve() =0;
    int calculate_cost(vector<int> solution){
        int cost = 0;
        for(int j=0; j<solution.size()-1; j++){
            cost += distances[solution[j]][solution[j+1]];
        }
        cost+=distances[solution[solution.size()-1]][solution[0]];
        for(int j=0;j<solution.size();j++){
            cost+=costs[solution[j]];
        }
        return cost;
    }
    string get_name(){
        return this->name;
    }
};

class RandomSearch : public Algo {
public:
    double time_limit;
    RandomSearch(vector<vector<int>> distances, vector<int> costs, int starting_node, double time_limit)
        : Algo(distances, costs, "RandomSearch", starting_node), time_limit(time_limit) {}
    
    Result solve() {
        while (true){
            clock_t start, end;
            start = clock();
            vector<int> worstSolution;
            int solution_size = this->distances.size();
            vector<int> current_solution = vector<int>(solution_size);
            iota(current_solution.begin(), current_solution.end(), 0);
            shuffle(current_solution.begin(), current_solution.end(), rng);
            int bestCost = calculate_cost(current_solution);
            vector<int> bestSolution = current_solution;
            while (true){
                end = clock();
                double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
                if(time_taken >= this->time_limit){
                    return Result(bestCost, 0, 0, bestSolution, worstSolution);
                }
                shuffle(current_solution.begin(), current_solution.end(), rng);
                int cost = calculate_cost(current_solution);
                if(cost < bestCost){
                    bestCost = cost;
                    bestSolution = current_solution;
                }
            }
        }
    }
};

class RandomWalk: public Algo {
public:
    double time_limit;
    RandomWalk(vector<vector<int>> distances, vector<int> costs, int starting_node, double time_limit)
        : Algo(distances, costs, "RandomWalk", starting_node), time_limit(time_limit) {}

    Result solve() {
        vector<int> worstSolution;
        int solution_size = this->distances.size();
        vector<int> current_solution = vector<int>(solution_size);
        iota(current_solution.begin(), current_solution.end(), 0);
        shuffle(current_solution.begin(), current_solution.end(), rng);
        int bestCost = calculate_cost(current_solution);
        vector<int> bestSolution = current_solution;
        clock_t start, end;
        start = clock();
        while (true){
            end = clock();
            double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
            if(time_taken >= this->time_limit){
                return Result(bestCost, 0, 0, bestSolution, worstSolution);
            }
            int i = rng() % solution_size;
            int j = rng() % solution_size;
            swap(current_solution[i], current_solution[j]);
            int cost = calculate_cost(current_solution);
            if(cost < bestCost){
                bestCost = cost;
                bestSolution = current_solution;
            }
        }
    }

};

class GreedyCycle: public Algo {
public:
    GreedyCycle(vector<vector<int>> distances, vector<int> costs, int starting_node)
        : Algo(distances, costs, "GreedyCycle", starting_node) {}

    Result solve() {
        vector<int> worstSolution;
        int solution_size = this->distances.size();
        vector<int> current_solution;
        vector<bool> visited(this->costs.size());
        current_solution.push_back(this->starting_node);
        visited[this->starting_node] = true;

        while(current_solution.size() < solution_size){
            int smallest_increase = INT32_MAX;
            int insert_index = -1;
            int insert_node = -1;


            for(int j=0; j<current_solution.size(); j++){  // Dla każdego nodea z cyklu
                int min_distance = INT32_MAX;
                int min_index = -1;
                for(int k=0; k<this->distances.size(); k++){ //znajdź najbliższy nieodwiedzony node
                    if(visited[k]) continue;
                    int curr = -this->distances[current_solution[j == 0 ? current_solution.size() - 1 : j - 1]][current_solution[j]] + this->distances[current_solution[j == 0 ? current_solution.size() - 1 : j - 1]][k] + this->distances[k][current_solution[j]] + this->costs[k];
                    if(curr < min_distance){
                        min_distance = curr;
                        min_index = k;
                    }
                }
                if(min_distance < smallest_increase){
                    smallest_increase = min_distance;
                    insert_index = j;
                    insert_node = min_index;
                }
            } // koniec
            current_solution.insert(current_solution.begin() + insert_index, insert_node);
            visited[insert_node] = true; 
        }
        return Result(0, 0, 0, current_solution, worstSolution);
    }
};



template <typename T>
struct generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        T value;
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        generator get_return_object() { return generator{handle_type::from_promise(*this)}; }
        void unhandled_exception() { std::terminate(); }
        std::suspend_always yield_value(T val)
        {
            value = val;
            return {};
        }
    };

    bool move_next() { return coro ? (coro.resume(), !coro.done()) : false; }
    T current_value() { return coro.promise().value; }

    generator(generator const &) = delete;
    generator(generator &&other) : coro(other.coro) { other.coro = {}; }
    ~generator()
    {
        if (coro)
            coro.destroy();
    }

private:
    generator(handle_type h) : coro(h) {}
    handle_type coro;
};

enum SearchType
{
    greedy,
    steepest
};
enum InitialSolutionType
{
    randomAlg,
    GC,
    randomWalk
};

enum ProblemInstance
{
    TSPA,
    TSPB,
    TSPC,
    TSPD
};

std::map<SearchType, std::string> SearchTypeStrings = {
    {greedy, "greedy"},
    {steepest, "steepest"}};

std::map<InitialSolutionType, std::string> InitialSolutionTypeStrings = {
    {randomAlg, "random"},
    {GC, "GreedyCycle"},
    {randomWalk, "randomWalk"}
};

std::map<ProblemInstance, std::string> ProblemInstanceStrings = {
    {TSPA, "TSPA"},
    {TSPB, "TSPB"},
    {TSPC, "TSPC"},
    {TSPD, "TSPD"}};

class LocalSearch : public Algo
{
public:
    SearchType searchType;
    InitialSolutionType initialSolutionType;
    int nPoints;
    LocalSearch(SearchType searchType, InitialSolutionType initialSolutionType, vector<vector<int>> distances, vector<int> costs, int starting_node)
        : Algo(distances, costs, "LS", starting_node), searchType(searchType), initialSolutionType(initialSolutionType)
    {
        this->name += "_" + SearchTypeStrings[searchType];
        this->name += "_" + InitialSolutionTypeStrings[initialSolutionType];
        nPoints = distances.size();
    }

    int calculate_cost(const vector<int> &solution)
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

    int fixIndex(int index, int solutionSize)
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

    Result solve()
    {
        vector<int> solution = getInitialSolution(this->initialSolutionType);
        // cout << "Initial solution created" << endl;
        // cout << solution.size() << endl;
        localSearch(&solution);
        return Result(calculate_cost(solution), 0, 0, solution, vector<int>());
    }

    void localSearch(vector<int> *solution)
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

    vector<int> getInitialSolution(InitialSolutionType ist)
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

    int calculateDelta(vector<int> &solution, vector<int> &move)
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

    void applyMove(vector<int> *solution, const vector<int> &move)
    { // modifies solution i think usuall  poinnter is enuogh
        int j = move[1];
        int k = move[2];
        reverse(solution->begin() + j, solution->begin() + k + 1);
    }

    generator<vector<int>> neighbourhoodGenerator(vector<int> &currentSolution)
    { // generate moves, that do an edge exchange
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


    // generator<vector<int>> intraEdgesNeighbourhoodGenerator(vector<int> &currentSolution)
    // {
    //     vector<int> temp_vec = {0, 0, 0, 0};
    //     vector<int> move = vector<int>(temp_vec);
    //     for (int i = 0; i < currentSolution.size(); i++)
    //     {
    //         int node1 = currentSolution[i];
    //         int node1_next = currentSolution[fixIndex(i + 1, currentSolution.size())];
    //         for (int j = i + 2; j < currentSolution.size(); j++)
    //         {
    //             int node2 = currentSolution[j];
    //             int node2_next = currentSolution[fixIndex(j + 1, currentSolution.size())];
    //             co_yield makeIntraMove(i, i + 1, j, fixIndex(j + 1, currentSolution.size()));
    //         }
    //     }
    // }

    vector<int> makeIntraMove(int edge1_first, int edge1_second, int edge2_first, int edge2_second)
    {
        return {edge1_first, edge1_second, edge2_first, edge2_second};
    }
};

vector<vector<int>> read_file(string filename) {
    vector<vector<int>> result;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        string cell;
        while (getline(ss, cell, ';')) {
            row.push_back(stoi(cell));
        }
        result.push_back(row);
    }
    return result;
}


vector<vector<int>> calcDistances(vector<vector<int>> data){
    vector<vector<int>> distances;
    for (int i = 0; i < data.size(); i++){
        vector<int> row;
        for (int j = 0; j < data.size(); j++){
            int x = data[i][0] - data[j][0];
            int y = data[i][1] - data[j][1];
            //round to nearest int
            float distance = round(sqrt(x*x + y*y));
            row.push_back(distance);
        }
        distances.push_back(row);
    }
    return distances;
}



int main(){

    string root_path = "./data/";
    vector<ProblemInstance> problemInstances = {TSPA};//, TSPB, TSPC, TSPD};
    vector<SearchType> searchTypes = {steepest, greedy};
    vector<InitialSolutionType> initialSolutionTypes = {randomAlg, GC, randomWalk};


    for(auto problemInstance: problemInstances){
        string file = root_path + ProblemInstanceStrings[problemInstance] + ".csv";
        auto data = read_file(file);
        auto distances = calcDistances(data);
        vector<int> costs;
        for(int i=0; i< data.size(); i++){
            costs.push_back(data[i][2]);
        }
        for(auto searchType: searchTypes){
            for(auto initialSolutionType: initialSolutionTypes){
                    cout << "Name: " << LocalSearch(searchType, initialSolutionType, distances, costs, rand() % distances.size()).get_name() << endl;
                    cout << "Problem instance: " << ProblemInstanceStrings[problemInstance] << endl;
                    Result algoResult = Result(INT32_MAX, 0, 0, vector<int>(), vector<int>());
                    double averageTime = 0;
                    for(int i=0; i<20; i++){
                        LocalSearch ls = LocalSearch(searchType, initialSolutionType, distances, costs, rand() % distances.size());
                        clock_t start, end;
                        start = clock();
                        vector<int> solution = ls.solve().bestSolution;
                        end = clock();
                        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
                        int cost = ls.calculate_cost(solution);
                        if(cost < algoResult.bestCost){
                            algoResult.bestCost = cost;
                            algoResult.bestSolution = solution;
                        }
                        if(cost > algoResult.worstCost){
                            algoResult.worstCost = cost;
                            algoResult.worstSolution = solution;
                        }
                        algoResult.averageCost += cost;
                        // cout << "Time taken: " << time_taken << endl;
                        averageTime += time_taken;
                    }
                    algoResult.averageCost /= 20;
                    cout << "Best cost: " << algoResult.bestCost << endl;
                    cout << "Worst cost: " << algoResult.worstCost << endl;
                    cout << "Average cost: " << algoResult.averageCost << endl;
                    averageTime /= 20;
                    cout << "Average time: " << averageTime << endl;
                    cout << "Best solution: ";
                    for(int i=0; i<algoResult.bestSolution.size(); i++){
                        cout << algoResult.bestSolution[i] << " ";
                    }
                    cout << endl;
                }
            }
        }

}