#include "a_star.h"

#include <memory>
#include <queue>
#include <set>
#include <algorithm>

namespace astar {

    struct PathNode {
        NodeID node;
        Cost total_cost;
        Cost estimated_cost;
        std::shared_ptr<PathNode> previous;
    };

    bool operator>(const PathNode& left, const PathNode& right) {
        return left.estimated_cost > right.estimated_cost;
    }

    // Build final path
    Result backtrace(PathNode current) {

        std::vector<NodeID> path;
        Cost totalCost = current.total_cost;

        while (true) {

            path.push_back(current.node);

            if (current.previous == nullptr)
                break;

            current = *current.previous;
        }

        std::reverse(path.begin(), path.end());

        return { path, totalCost };
    }

    Result a_star(AdjacencyList& adjacency, NodeID start, NodeID target, HeuristicFn& heuristic) {

        std::set<NodeID> visited;

        std::priority_queue<
            PathNode,
            std::vector<PathNode>,
            std::greater<>
        > boundary;

        // Push start node
        boundary.push({
            start,
            0,
            heuristic(start, target),
            nullptr
        });

        while (!boundary.empty()) {

            PathNode current = boundary.top();
            boundary.pop();

            // Skip if already visited
            if (visited.find(current.node) != visited.end())
                continue;

            visited.insert(current.node);

            // Goal reached
            if (current.node == target) {
                return backtrace(current);
            }

            // Expand neighbors
            for (const auto& edge : adjacency[current.node]) {

                NodeID neighbor = edge.first;
                Cost weight = edge.second;

                if (visited.find(neighbor) != visited.end())
                    continue;

                Cost newCost = current.total_cost + weight;
                Cost estimated = newCost + heuristic(neighbor, target);

                boundary.push({neighbor, newCost, estimated, std::make_shared<PathNode>(current)});
            }
        }

        // No path found
        return { {}, 0 };
    }
}
