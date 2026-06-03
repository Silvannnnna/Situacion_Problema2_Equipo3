#ifndef NETWORKBUILDER_H
#define NETWORKBUILDER_H
#include <vector>
#include <string>

// Builds the Minimum Spanning Tree (MST) using Prim or Kruskal.
class NetworkBuilder {
public:
    struct Edge {
        int u, v, weight;
    };

    // Returns the MST edges given an N×N distance matrix.
    std::vector<Edge> buildMST(int N, const std::vector<std::vector<int>>& dist);

    // Formats the MST result as the required output string.
    std::string formatResult(const std::vector<Edge>& mst);
};

#endif
