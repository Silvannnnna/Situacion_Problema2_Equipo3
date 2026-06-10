#include "../include/NetworkBuilder.h"

#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>

// Algoritmo de Kruskal para construir el Árbol de Expansión Mínima (MST) a partir de la matriz de distancias
// Complejidad: O(n log n) 
std::vector<NetworkBuilder::Edge> NetworkBuilder::buildMST(
    int N, const std::vector<std::vector<int>>& dist)
{
    // Recolecta todas las aristas del triangulo superior
    std::vector<Edge> edges;
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (dist[i][j] > 0)
                edges.push_back({i, j, dist[i][j]});

    // Se ordena por peso ascendente
    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.weight < b.weight; });

    // Aplica Union-Find donde cada nodo es su propio padre al inicio
    std::vector<int> parent(N);
    std::iota(parent.begin(), parent.end(), 0);

    // Funcion de busqueda con compresion de caminos
    std::function<int(int)> find = [&](int x) -> int {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    };

    // Agrega aristas que no formen ciclos hasta completar el MST
    std::vector<Edge> mst;
    for (const auto& e : edges) {
        int pu = find(e.u), pv = find(e.v);
        if (pu != pv) {
            parent[pu] = pv;
            mst.push_back(e);
            if (static_cast<int>(mst.size()) == N - 1) {
                break;
            }
        }
    }
    return mst; // regresa el MST construido
}
// Formatea el resultado del MST en una cadena de texto
// Complejidad: O(n)
std::string NetworkBuilder::formatResult(const std::vector<Edge>& mst) {
    std::ostringstream oss;
    for (const auto& e : mst) { 
        oss << "(" << static_cast<char>('A' + e.u) << ", "
            << static_cast<char>('A' + e.v) << ")\n";
    }
    return oss.str();
}
