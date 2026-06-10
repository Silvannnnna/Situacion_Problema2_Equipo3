#ifndef ROUTINGANDFLOW_H
#define ROUTINGANDFLOW_H

#include <vector>
#include <string>

// Solves the TSP (Held-Karp / backtracking) and Max Flow (Ford-Fulkerson).
class RoutingAndFlow {
public:
    // Returns the shortest Hamiltonian cycle order starting from node 0.
    /** Resuelve el TSP con backtracking. Devuelve el ciclo hamiltoniano mas corto desde el nodo 0. */
    std::vector<int> SolveTSP(int N, const std::vector<std::vector<int>>& dist);

    // Returns the maximum flow from source to sink.
    int maxFlow(int N, const std::vector<std::vector<int>>& capacity,
                int source, int sink);

    // Formats TSP result as the required output string.
    std::string formatTSP(const std::vector<int>& tour);

    // Formats max-flow result as the required output string.
    std::string formatFlow(int flow);
};

#endif