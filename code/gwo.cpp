#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>

using namespace std;

namespace gwo {

    struct Result {
        vector<int> path;
        int cost;
    };

    // Calculate total cost of a path
    int calculateCost(const vector<int>& path, const vector<vector<pair<int,int>>>& graph) {

        int total = 0;

        for (int i = 0; i < static_cast<int>(path.size()) - 1; i++) {

            int u = path[i];
            int v = path[i + 1];

            bool found = false;

            for (const auto& edge : graph[u]) {

                if (edge.first == v) {
                    total += edge.second;
                    found = true;
                    break;
                }
            }

            if (!found)
                return INT_MAX;
        }

        return total;
    }

    // Create random valid path from source to target
    vector<int> randomPath(int source, int target, const vector<vector<pair<int,int>>>& graph) {

        vector<int> path;
        vector<bool> visited(graph.size(), false);

        int current = source;

        path.push_back(current);
        visited[current] = true;

        int safety = 0;
        int maxSteps = static_cast<int>(graph.size()) * 3;

        while (current != target && safety < maxSteps) {

            if (graph[current].empty()) {
                path.clear();
                return path;
            }

            vector<int> candidates;

            for (const auto& edge : graph[current]) {

                int neighbor = edge.first;

                if (!visited[neighbor]) {
                    candidates.push_back(neighbor);
                }
            }

            // Dead end
            if (candidates.empty()) {
                path.clear();
                return path;
            }

            int randomIndex = rand() % candidates.size();
            int nextNode = candidates[randomIndex];

            current = nextNode;
            visited[current] = true;
            path.push_back(current);

            safety++;
        }

        // Failed to reach target
        if (current != target) {
            path.clear();
        }

        return path;
    }

    // Grey Wolf Optimization approximation
    Result gwoShortestPath(const vector<vector<pair<int,int>>>& graph, int source, int target, int wolves, int iterations) {

        vector<int> bestPath;
        int bestCost = INT_MAX;

        for (int iteration = 0; iteration < iterations; iteration++) {

            for (int wolf = 0; wolf < wolves; wolf++) {

                vector<int> candidatePath =
                    randomPath(source, target, graph);

                if (candidatePath.empty())
                    continue;

                int candidateCost =
                    calculateCost(candidatePath, graph);

                if (candidateCost < bestCost) {

                    bestCost = candidateCost;
                    bestPath = candidatePath;
                }
            }
        }

        return {bestPath, bestCost};
    }
}
