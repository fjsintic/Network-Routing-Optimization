// a_star.h
#pragma once
#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

namespace astar {
    typedef size_t NodeID;
    typedef float Cost;
    typedef std::pair<const NodeID, Cost> Edge;
    typedef std::unordered_map<NodeID, std::unordered_map<NodeID, Cost>> AdjacencyList;
    struct Result {
    std::vector<NodeID> path;
    Cost cost;
    };

    typedef const std::function<Cost(NodeID, NodeID)> HeuristicFn;

    Result a_star(AdjacencyList& adjacency,
              NodeID start,
              NodeID target,
              HeuristicFn& heuristic);
}
