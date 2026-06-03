#include "GeoDistricts.h"

#include <sstream>
#include <cmath>
#include <limits>

int GeoDistricts::nearestNeighbor(
    const std::vector<std::pair<int,int>>& centrals,
    std::pair<int,int> query)
{
    // TODO: find index of closest central to query point
    (void)centrals; (void)query;
    return 0;
}

std::vector<std::vector<std::pair<double,double>>>
GeoDistricts::voronoiPolygons(const std::vector<std::pair<int,int>>& coords) {
    // TODO: build Voronoi regions (discretization or Fortune's algorithm)
    (void)coords;
    return {};
}

std::string GeoDistricts::formatResult(
    const std::vector<std::pair<int,int>>& coords,
    const std::vector<std::pair<int,int>>& centrals)
{
    // TODO: for each colony, print nearest central
    (void)coords; (void)centrals;
    std::ostringstream oss;
    return oss.str();
}
