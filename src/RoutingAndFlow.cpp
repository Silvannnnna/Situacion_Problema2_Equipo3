#include "../include/RoutingAndFlow.h"

#include <sstream>
#include <functional>
#include <queue>
#include <algorithm>
#include <climits>

std::vector<int> RoutingAndFlow::solveTSP( //BACKTRACKING TSP SOLUTION: Pending to check
    int N, const std::vector<std::vector<int>>& dist)
{
    std::vector<int> bestTour;
    std::vector<int> currentPath;
    std::vector<bool> visited(N, false);
    int minCost = INT_MAX;

    // Helper function for backtracking TSP
    std::function<void(int, int)> backtrack = [&](int lastNode, int cost) {
        // Base case: all nodes visited
        if (static_cast<int>(currentPath.size()) == N) {
            int totalCost = cost + dist[lastNode][0]; // Return to start
            if (totalCost < minCost) {
                minCost = totalCost;
                bestTour = currentPath;
            }
            return;
        }

        // Pruning: skip if current cost exceeds best found
        if (cost >= minCost) {
            return;
        }

        // Try visiting each unvisited node
        for (int next = 0; next < N; ++next) {
            if (!visited[next]) {
                visited[next] = true;
                currentPath.push_back(next);
                backtrack(next, cost + dist[lastNode][next]);
                currentPath.pop_back();
                visited[next] = false;
            }
        }
    };

    visited[0] = true;
    currentPath.push_back(0);
    backtrack(0, 0);
    return bestTour;
}
//Implemeentation of maxFlow using Ford-Fulkerson (Edmonds-Karp) algorithm
int RoutingAndFlow::maxFlow(
    int N, const std::vector<std::vector<int>>& capacity,
    int source, int sink)
{
    // Ford-Fulkerson using BFS (Edmonds-Karp)
    std::vector<std::vector<int>> residual = capacity;
    int maxFlowValue = 0;

    // Helper function to find augmenting path using BFS
    auto findAugmentingPath = [&]() -> std::vector<int> {
        std::vector<int> parent(N, -1);
        std::vector<bool> visited(N, false);
        std::queue<int> q;

        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // Check all adjacent nodes
            for (int v = 0; v < N; ++v) {
                if (!visited[v] && residual[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);

                    if (v == sink) {
                        // Found path, reconstruct it
                        std::vector<int> path;
                        int curr = sink;
                        while (curr != -1) {
                            path.push_back(curr);
                            curr = parent[curr];
                        }
                        std::reverse(path.begin(), path.end());
                        return path;
                    }
                }
            }
        }
        return {}; // No augmenting path found
    };

    // Find augmenting paths and update flow
    std::vector<int> path;
    while (!(path = findAugmentingPath()).empty()) {
        // Find minimum capacity along the path (bottleneck)
        int pathFlow = INT_MAX;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            pathFlow = std::min(pathFlow, residual[path[i]][path[i + 1]]);
        }

        // Update residual capacities
        for (size_t i = 0; i < path.size() - 1; ++i) {
            residual[path[i]][path[i + 1]] -= pathFlow;
            residual[path[i + 1]][path[i]] += pathFlow;
        }

        maxFlowValue += pathFlow;
    }

    return maxFlowValue;
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
