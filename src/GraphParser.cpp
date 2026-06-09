#include "../include/GraphParser.h"

#include <fstream>
#include <stdexcept>

// Lee el archivo de entrada y expone los datos del grafo al resto de modulos
// Complejidad: O(N^2)
void GraphParser::parse(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }

    file >> N; // lee el num de colonias 

    // Lee la matriz de distancias 
    dist.assign(N, std::vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            file >> dist[i][j];

    // Lee la matriz de capacidades  
    capacity.assign(N, std::vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            file >> capacity[i][j];

    // Lee las coordenadas de cada colonia
    coords.resize(N);
    for (int i = 0; i < N; ++i) {
        char lp, comma, rp;
        file >> lp >> coords[i].first >> comma >> coords[i].second >> rp;
    }
}
