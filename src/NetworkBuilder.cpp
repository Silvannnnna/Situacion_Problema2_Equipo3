#include "NetworkBuilder.h"

#include <sstream>

std::vector<NetworkBuilder::Edge> NetworkBuilder::buildMST(
    int N, const std::vector<std::vector<int>>& dist)
{
    // TODO: implement Prim or Kruskal
    (void)N; (void)dist;
    return {};
}

std::string NetworkBuilder::formatResult(const std::vector<Edge>& mst) {
    std::ostringstream oss;
    for (const auto& e : mst) {
        oss << "(" << static_cast<char>('A' + e.u) << ", "
            << static_cast<char>('A' + e.v) << ")\n";
    }
    return oss.str();
}
