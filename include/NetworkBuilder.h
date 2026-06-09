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

    std::vector<Edge> buildMST(int N, const std::vector<std::vector<int>>& dist); // construye el MST usando Kruskal
    std::string formatResult(const std::vector<Edge>& mst); // formatea el resultado del MST en una cadena de texto
};

#endif
