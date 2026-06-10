// RoutingAndFlow.cpp — TSP backtracking y Max Flow Ford-Fulkerson (Edmonds-Karp)
#include "../include/RoutingAndFlow.h"

#include <sstream>
#include <queue>
#include <algorithm>
#include <climits>

struct TspState {
    std::vector<int> bestTour;
    std::vector<int> currentPath;
    std::vector<bool> visited;
    int minCost;
};

static void tsp_backtrack(
    TspState& state, int lastNode, int cost,
    int N, const std::vector<std::vector<int>>& dist)
{
    if (static_cast<int>(state.currentPath.size()) == N) {
        int totalCost = cost + dist[lastNode][0];
        if (totalCost < state.minCost) {
            state.minCost = totalCost;
            state.bestTour = state.currentPath;
        }
        return;
    }

    if (cost >= state.minCost) {
        return;
    }

    for (int next = 0; next < N; ++next) {
        if (!state.visited[next]) {
            state.visited[next] = true;
            state.currentPath.push_back(next);
            tsp_backtrack(state, next, cost + dist[lastNode][next], N, dist);
            state.currentPath.pop_back();
            state.visited[next] = false;
        }
    }
}

std::vector<int> RoutingAndFlow::SolveTSP(
    int N, const std::vector<std::vector<int>>& dist)
{
    TspState state;
    state.visited.assign(N, false);
    state.minCost = INT_MAX;

    state.visited[0] = true;
    state.currentPath.push_back(0);
    tsp_backtrack(state, 0, 0, N, dist);
    return state.bestTour;
}

static std::vector<int> reconstruct_path(int sink, const std::vector<int>& parent) {
    std::vector<int> path;
    int curr = sink;
    while (curr != -1) {
        path.push_back(curr);
        curr = parent[curr];
    }
    std::reverse(path.begin(), path.end());
    return path;
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
                    return reconstruct_path(sink, parent);
                }
            }
        }
    }
    return {};
}

int RoutingAndFlow::MaxFlow(
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

std::string RoutingAndFlow::FormatTSP(const std::vector<int>& tour) {
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

std::string RoutingAndFlow::FormatFlow(int flow) {
    return std::to_string(flow) + "\n";
}
