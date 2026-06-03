#pragma once

#include <string>
#include <vector>

// Parses input.txt and exposes the graph data to other modules.
class GraphParser {
public:
    int N;
    std::vector<std::vector<int>> dist;      // N×N distance matrix
    std::vector<std::vector<int>> capacity;  // N×N capacity matrix
    std::vector<std::pair<int,int>> coords;  // (x, y) per colony

    // Reads all data from the given file path.
    void parse(const std::string& filePath);
};
