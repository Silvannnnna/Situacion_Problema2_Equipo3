#ifndef NETWORKBUILDER_H
#define NETWORKBUILDER_H
#include <vector>
#include <string>

// Construye el Árbol de Expansión Mínima (MST) de la red de colonias usando el algoritmo de Kruskal con Union-Find y compresion de caminos
class NetworkBuilder {
public:
    struct Edge { // representa una arista entre dos nodos con un peso (distancia)
        int u, v, weight;
    };

    /** Construye el MST usando el algoritmo de Kruskal. */
    std::vector<Edge> buildMST(int N, const std::vector<std::vector<int>>& dist);

    /** Formatea el resultado del MST como cadena de texto. */
    std::string formatResult(const std::vector<Edge>& mst);
};

#endif
