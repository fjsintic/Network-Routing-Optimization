#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

namespace bellmanford {

    struct Edge {
        int source;
        int destination;
        int weight;
    };

    struct Result {
        vector<int> path;
        int cost;
    };

    Result bellmanFord(vector<Edge>& graph, int vertices, int source, int target) {

        vector<int> distance(vertices, INT_MAX);
        vector<int> parent(vertices, -1);

        distance[source] = 0;

        // Relax all edges V-1 times
        for (int i = 0; i < vertices - 1; i++) {

            bool updated = false;

            for (const Edge& e : graph) {

                if (distance[e.source] != INT_MAX &&
                    distance[e.source] + e.weight < distance[e.destination]) {

                    distance[e.destination] =
                        distance[e.source] + e.weight;

                    parent[e.destination] = e.source;
                    updated = true;
                }
            }

            // Optimization: stop early if no updates
            if (!updated)
                break;
        }

        // Check for negative cycles
        for (const Edge& e : graph) {

            if (distance[e.source] != INT_MAX &&
                distance[e.source] + e.weight < distance[e.destination]) {

                cout << "Graph contains negative weight cycle\n";
                return {{}, INT_MAX};
            }
        }

        vector<int> path;

        // Reconstruct path
        if (distance[target] != INT_MAX) {

            for (int current = target;
                 current != -1;
                 current = parent[current]) {

                path.push_back(current);
            }

            reverse(path.begin(), path.end());
        }

        return {path, distance[target]};
    }
}
