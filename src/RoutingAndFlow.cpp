#include "../include/RoutingAndFlow.h"

#include <sstream>
#include <functional>
#include <queue>
#include <algorithm>
#include <climits>

std::vector<int> RoutingAndFlow::SolveTSP(
    int N, const std::vector<std::vector<int>>& dist)
{
    std::vector<int> bestTour;
    std::vector<int> currentPath;
    std::vector<bool> visited(N, false);
    int minCost = INT_MAX;

    std::function<void(int, int)> backtrack = [&](int lastNode, int cost) {
        if (static_cast<int>(currentPath.size()) == N) {
            int totalCost = cost + dist[lastNode][0];
            if (totalCost < minCost) {
                minCost = totalCost;
                bestTour = currentPath;
            }
            return;
        }

        if (cost >= minCost) {
            return;
        }

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

static std::vector<int> find_augmenting_path(
    int N, int source, int sink,
    const std::vector<std::vector<int>>& residual)
{
    std::vector<int> parent(N, -1);
    std::vector<bool> visited(N, false);
    std::queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < N; ++v) {
            if (!visited[v] && residual[u][v] > 0) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);

                if (v == sink) {
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
    return {};
}

int RoutingAndFlow::maxFlow(
    int N, const std::vector<std::vector<int>>& capacity,
    int source, int sink)
{
    std::vector<std::vector<int>> residual = capacity;
    int maxFlowValue = 0;

    std::vector<int> path;
    while (!(path = find_augmenting_path(N, source, sink, residual)).empty()) {
        int pathFlow = INT_MAX;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            pathFlow = std::min(pathFlow, residual[path[i]][path[i + 1]]);
        }

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
    for (int node : tour) {
        oss << static_cast<char>('A' + node) << " -> ";
    }
    if (!tour.empty()) {
        oss << static_cast<char>('A' + tour.front());
    }
    oss << "\n";
    return oss.str();
}

std::string RoutingAndFlow::formatFlow(int flow) {
    return std::to_string(flow) + "\n";
}
