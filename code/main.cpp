#include "graph_generator.cpp"

// Extra Libraries
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <ctime>

// Included Algorithms . . .
#include "dijkstra.cpp"
#include "Astar.cpp"
#include "bellman_ford.cpp"
#include "gwo.cpp"

using namespace std;

int main() {

    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Graph parameters
    int V = 10000;
    int E = 30000;
    int source = 0;
    int target = 666;

    // Generate graph
    auto graph = generateGraph(V, E);


    /*
        Dijkstra Algorithm
    */

    // Time the Algorithm
    auto start1 = chrono::high_resolution_clock::now();
    DijkstraResult dijkstraResult = dijkstra(V, source, target, graph);
    auto end1 = chrono::high_resolution_clock::now();

    double ms1 = chrono::duration<double>(end1 - start1).count();

    cout << "\nDijkstra Algorithm Path From Node "
         << source << " to Node " << target << " -----\n";
    if (dijkstraResult.path.empty()) {
        cout << "No path found\n";
    }
    else {
        cout << "Path: ";
        for (int node : dijkstraResult.path) {
            cout << node << " -> ";
        }
        cout << "\n";
    }


    /*
        A* Algorithm
    */

    /*
        Heuristics - Minimum Edge Weight Scaling 
        -> Avoids overstimulating because of the random graph, essentially 
           functions similarly to Dijkstra's Algorithm with a barely slower time completion.
        -> Runtime: Decent, Accuracy: Identical to Dijkstra's Algorithm
    */ 
    
    auto heuristic = [](astar::NodeID a, astar::NodeID b) -> astar::Cost {
        return 1;
    };
    
    astar::AdjacencyList adjList;

    for (int u = 0; u < V; u++) {
        for (auto& edge : graph[u]) {
            adjList[u][edge.first] = static_cast<astar::Cost>(edge.second);
        }
    }

    // Time the Algorithm

    auto start2 = chrono::high_resolution_clock::now();
    astar::Result result = astar::a_star(adjList, source, target, heuristic);
    auto end2 = chrono::high_resolution_clock::now();

    double ms2 = chrono::duration<double>(end2 - start2).count();

    cout << "\nA* Algorithm Path From Node "
         << source << " to Node " << target << " -----\n";
    if (result.path.empty()) {
        cout << "No path found\n";
    }
    else {
        cout << "Path: ";
        for (auto node : result.path) {
            cout << node << " -> ";
        }
        cout << "NULL\n";
    }


    /*
        Bellman-Ford Algorithm
    */

    vector<bellmanford::Edge> bfGraph;

    for (int u = 0; u < V; u++) {
        for (auto& edge : graph[u]) {
            bfGraph.push_back({u, edge.first, edge.second});
        }
    }
    
    // Time the Algorithm

    auto start3 = chrono::high_resolution_clock::now();
    bellmanford::Result bfResult =
        bellmanford::bellmanFord(bfGraph, V, source, target);
    auto end3 = chrono::high_resolution_clock::now();

    double ms3 = chrono::duration<double>(end3 - start3).count();

    cout << "\nBellman-Ford Algorithm Path From Node "
         << source << " to Node " << target << " -----\n";
    if (bfResult.path.empty()) {
        cout << "No path found\n";
    }
    else {
        cout << "Path: ";
        for (int node : bfResult.path) {
            cout << node << " -> ";
        }
        cout << "NULL\n";
    }


    /*
        Grey Wolf Optimization Algorithm
    */
    
    /*    
        Wolf/Iteration Specifications
        -> This gives 300,000 candidate paths tested, which will be 
           enough to accurately find the best path.
        -> Runtime: Moderate, Accuracy: Decent
    */
    
    int wolves = 300;
    int iterations = 1000;

    // Time the Algorithm
    auto start4 = chrono::high_resolution_clock::now();
    gwo::Result gwoResult =
        gwo::gwoShortestPath(graph, source, target, wolves, iterations);
    auto end4 = chrono::high_resolution_clock::now();

    double ms4 = chrono::duration<double>(end4 - start4).count();

    cout << "\nGWO Algorithm Path From Node "
         << source << " to Node " << target << " -----\n";
    if (gwoResult.path.empty()) {
        cout << "No path found\n";
    }
    else {
        cout << "Path: ";
        for (auto node : gwoResult.path) {
            cout << node << " -> ";
        }
        cout << "NULL\n";
    }


    /*
        OUTPUT TABLE:

        ===================== ALGORITHM =======================
        Algorithm:     Time (s):      Cost:
        -----          -----          -----
        
        */

    cout << "\n===================== ALGORITHM =======================\n";

    cout << left << setw(15) << "Algorithm"
         << setw(15) << "Time (s)"
         << setw(15) << "Cost" << "\n";

    cout << "--------------------------------------------------------\n";

    cout << left << setw(15) << "Dijkstra"
         << setw(15) << ms1
         << setw(15) << dijkstraResult.cost << "\n";

    cout << left << setw(15) << "A*"
         << setw(15) << ms2
         << setw(15) << result.cost << "\n";

    cout << left << setw(15) << "Bellman-Ford"
         << setw(15) << ms3
         << setw(15) << bfResult.cost << "\n";

    cout << left << setw(15) << "GWO"
         << setw(15) << ms4
         << setw(15) << gwoResult.cost << "\n\n";

    return 0;
}
