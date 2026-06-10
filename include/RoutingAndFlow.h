#ifndef ROUTINGANDFLOW_H
#define ROUTINGANDFLOW_H

#include <vector>
#include <string>

/** Resuelve el TSP (backtracking) y flujo maximo (Ford-Fulkerson / Edmonds-Karp). */
class RoutingAndFlow {
public:
    /** Resuelve el TSP con backtracking. Devuelve el ciclo hamiltoniano mas corto desde el nodo 0. */
    std::vector<int> SolveTSP(int N, const std::vector<std::vector<int>>& dist);

    /** Calcula el flujo maximo de source a sink usando Ford-Fulkerson (Edmonds-Karp). */
    int MaxFlow(int N, const std::vector<std::vector<int>>& capacity,
                int source, int sink);

    /** Formatea el resultado del TSP como cadena de texto. */
    std::string FormatTSP(const std::vector<int>& tour);

    /** Formatea el flujo maximo como cadena de texto. */
    std::string FormatFlow(int flow);
};

#endif
