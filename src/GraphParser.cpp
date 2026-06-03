#include "../include/GraphParser.h"

#include <fstream>
#include <stdexcept>

void GraphParser::parse(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open input file: " + filePath);
    }

    file >> N;

    dist.assign(N, std::vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            file >> dist[i][j];

    capacity.assign(N, std::vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            file >> capacity[i][j];

    coords.resize(N);
    for (int i = 0; i < N; ++i) {
        char lp, comma, rp;
        file >> lp >> coords[i].first >> comma >> coords[i].second >> rp;
    }
}
