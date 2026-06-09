#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H
#include <string>
#include <vector>

// Lee el archivo de entrada y expone los datos del grafo al resto de módulos
class GraphParser {
public:
    int N; // numero de colonias
    std::vector<std::vector<int>> dist; // matriz de distancias 
    std::vector<std::vector<int>> capacity; // matriz de capacidades 
    std::vector<std::pair<int,int>> coords; // coordenadas de cada colonia

    void parse(const std::string& filePath);
};

#endif
