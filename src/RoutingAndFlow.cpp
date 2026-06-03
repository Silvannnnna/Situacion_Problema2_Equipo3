#include "RoutingAndFlow.h"

#include <sstream>

std::vector<int> RoutingAndFlow::solveTSP(
    int N, const std::vector<std::vector<int>>& dist)
{
    // TODO: implement Held-Karp or backtracking TSP
    (void)N; (void)dist;
    return {};
}

int RoutingAndFlow::maxFlow(
    int N, const std::vector<std::vector<int>>& capacity,
    int source, int sink)
{
    // TODO: implement Ford-Fulkerson (Edmonds-Karp)
    (void)N; (void)capacity; (void)source; (void)sink;
    return 0;
}

std::string RoutingAndFlow::formatTSP(const std::vector<int>& tour) {
    std::ostringstream oss;
    for (int node : tour)
        oss << static_cast<char>('A' + node) << " -> ";
    if (!tour.empty())
        oss << static_cast<char>('A' + tour.front());
    oss << "\n";
    return oss.str();
}

std::string RoutingAndFlow::formatFlow(int flow) {
    return std::to_string(flow) + "\n";
}
