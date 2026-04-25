#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

// Infinity constant
const int INF = numeric_limits<int>::max();

struct DijkstraResult {
    vector<int> path;
    int cost;
};

DijkstraResult dijkstra(int n, int source, int target, const vector<vector<pair<int,int>>>& adj) {

    vector<int> dist(n, INF);
    vector<int> parent(n, -1);

    dist[source] = 0;

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    pq.push({0, source});

    while (!pq.empty()) {

        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Skip outdated entries
        if (d > dist[u])
            continue;

        // Stop once target is reached
        if (u == target)
            break;

        // Relax edges
        for (const auto& edge : adj[u]) {

            int v = edge.first;
            int weight = edge.second;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {

                dist[v] = dist[u] + weight;
                parent[v] = u;

                pq.push({dist[v], v});
            }
        }
    }

    vector<int> path;

    // Reconstruct path
    if (dist[target] != INF) {

        for (int current = target; current != -1; current = parent[current]) {
            path.push_back(current);
        }

        reverse(path.begin(), path.end());
    }

    return {path, dist[target]};
}
