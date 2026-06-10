#include "../include/GeoDistricts.h"

#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>

namespace {
    double distance_squared(std::pair<int,int> a, std::pair<int,int> b) {
        double dx = static_cast<double>(a.first) - b.first;
        double dy = static_cast<double>(a.second) - b.second;
        return dx * dx + dy * dy;
    }

    std::string label(int index) {
        std::string result;
        index++;
        while (index > 0) {
            int rem = (index - 1) % 26;
            result.insert(result.begin(), static_cast<char>('A' + rem));
            index = (index - 1) / 26;
        }
        return result;
    }

    bool inside_half_plane(std::pair<double,double> p, double a, double b, double c) {
        const double eps = 1e-9;
        return a * p.first + b * p.second <= c + eps;
    }

    std::pair<double,double> line_intersection(
        std::pair<double,double> p1,
        std::pair<double,double> p2,
        double a, double b, double c
    ) {
        double v1 = a * p1.first + b * p1.second;
        double v2 = a * p2.first + b * p2.second;
        double t = (c - v1) / (v2 - v1);
        return {
            p1.first + t * (p2.first - p1.first),
            p1.second + t * (p2.second - p1.second)
        };
    }

    std::vector<std::pair<double,double>> clip_polygon(
        const std::vector<std::pair<double,double>>& polygon,
        double a, double b, double c
    ) {
        std::vector<std::pair<double,double>> result;
        if (polygon.empty()) {
            return result;
        }

        for (size_t i = 0; i < polygon.size(); i++) {
            auto current = polygon[i];
            auto next    = polygon[(i + 1) % polygon.size()];

            bool currentInside = inside_half_plane(current, a, b, c);
            bool nextInside    = inside_half_plane(next,    a, b, c);

            if (currentInside && nextInside) {
                result.push_back(next);
            } else if (currentInside && !nextInside) {
                result.push_back(line_intersection(current, next, a, b, c));
            } else if (!currentInside && nextInside) {
                result.push_back(line_intersection(current, next, a, b, c));
                result.push_back(next);
            }
        }
        return result;
    }
}


int GeoDistricts::nearestNeighbor(
    const std::vector<std::pair<int,int>>& centrals,
    std::pair<int,int> query,
    int excludeIndex
) {
    if (centrals.empty()) {
        return -1;
    }

    int    nearestIndex  = -1;
    double bestDistance  = std::numeric_limits<double>::max();

    for (int i = 0; i < static_cast<int>(centrals.size()); i++) {
        if (i == excludeIndex) {
            continue;
        }

        double currentDistance = distance_squared(centrals[i], query);
        if (currentDistance < bestDistance) {
            bestDistance  = currentDistance;
            nearestIndex  = i;
        }
    }
    return nearestIndex;
}


std::vector<std::vector<std::pair<double,double>>>
GeoDistricts::voronoiPolygons(const std::vector<std::pair<int,int>>& coords) {
    std::vector<std::vector<std::pair<double,double>>> regions;
    if (coords.empty()) {
        return regions;
    }

    int minX = coords[0].first;
    int maxX = coords[0].first;
    int minY = coords[0].second;
    int maxY = coords[0].second;

    for (const auto& p : coords) {
        minX = std::min(minX, p.first);
        maxX = std::max(maxX, p.first);
        minY = std::min(minY, p.second);
        maxY = std::max(maxY, p.second);
    }

    double width  = static_cast<double>(maxX - minX);
    double height = static_cast<double>(maxY - minY);
    double margin = std::max(width, height) * 0.25 + 10.0;

    double left   = minX - margin;
    double right  = maxX + margin;
    double bottom = minY - margin;
    double top    = maxY + margin;

    for (int i = 0; i < static_cast<int>(coords.size()); i++) {
        std::vector<std::pair<double,double>> polygon = {
            {left, bottom}, {right, bottom}, {right, top}, {left, top}
        };

        double xi = coords[i].first;
        double yi = coords[i].second;

        for (int j = 0; j < static_cast<int>(coords.size()); j++) {
            if (i == j) {
                continue;
            }
            double xj = coords[j].first;
            double yj = coords[j].second;
            double a = 2.0 * (xj - xi);
            double b = 2.0 * (yj - yi);
            double c = (xj*xj + yj*yj) - (xi*xi + yi*yi);
            polygon = clip_polygon(polygon, a, b, c);
            if (polygon.empty()) {
                break;
            }
        }
        regions.push_back(polygon);
    }
    return regions;
}


std::string GeoDistricts::formatResult(
    const std::vector<std::pair<int,int>>& coords,
    const std::vector<std::pair<int,int>>& centrals
) {
    std::ostringstream oss;
    if (coords.empty() || centrals.empty()) {
        return oss.str();
    }

    for (int i = 0; i < static_cast<int>(coords.size()); i++) {
        int nearest = nearestNeighbor(centrals, coords[i], i);

        oss << label(i) << " -> ";
        oss << (nearest == -1 ? "N/A" : label(nearest));

        if (i + 1 < static_cast<int>(coords.size())) {
            oss << "\n";
        }
    }
    return oss.str();
}
