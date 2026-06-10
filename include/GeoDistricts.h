#ifndef GEODISTRICTS_H
#define GEODISTRICTS_H
#include <vector>
#include <string>
#include <utility>

// Nearest-neighbor search and Voronoi-based district assignment.
class GeoDistricts {
public:
    // Returns the index of the nearest central node for the given query point.
    int nearestNeighbor(const std::vector<std::pair<int,int>>& centrals,
                        std::pair<int,int> query,
                        int excludeIndex = -1);

    // Returns a polygon approximation (list of vertices) for each central's
    // Voronoi region over the bounding box of coords.
    std::vector<std::vector<std::pair<double,double>>>
    voronoiPolygons(const std::vector<std::pair<int,int>>& coords);

    // Formats district assignment as the required output string.
    std::string formatResult(const std::vector<std::pair<int,int>>& coords,
                             const std::vector<std::pair<int,int>>& centrals);
};

#endif