#include <gtest/gtest.h>
#include <fstream>

#include "GraphParser.h"
#include "NetworkBuilder.h"
#include "RoutingAndFlow.h"
#include "GeoDistricts.h"

// Escribe un archivo temporal con el contenido dado y devuelve su ruta
static std::string writeTempInput(const std::string& content) {
    std::string path = "/tmp/test_input_graph.txt";
    std::ofstream f(path);
    f << content;
    return path;
}

// ── GraphParser ──────────────────────────────────────────────────────────────

TEST(GraphParserTest, ParsesN) {
    auto path = writeTempInput("2\n0 5\n5 0\n0 3\n3 0\n(0,0)\n(1,1)\n");
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.N, 2);
}

TEST(GraphParserTest, ParsesDistMatrix) {
    auto path = writeTempInput("2\n0 5\n5 0\n0 3\n3 0\n(0,0)\n(1,1)\n");
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.dist[0][1], 5);
    EXPECT_EQ(p.dist[1][0], 5);
    EXPECT_EQ(p.dist[0][0], 0);
}

TEST(GraphParserTest, ParsesCapacityMatrix) {
    auto path = writeTempInput("2\n0 5\n5 0\n0 3\n3 0\n(0,0)\n(1,1)\n");
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.capacity[0][1], 3);
    EXPECT_EQ(p.capacity[1][0], 3);
}

TEST(GraphParserTest, ParsesCoords) {
    auto path = writeTempInput("2\n0 5\n5 0\n0 3\n3 0\n(10,20)\n(30,40)\n");
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.coords[0].first,  10);
    EXPECT_EQ(p.coords[0].second, 20);
    EXPECT_EQ(p.coords[1].first,  30);
    EXPECT_EQ(p.coords[1].second, 40);
}

TEST(GraphParserTest, ThrowsOnMissingFile) {
    GraphParser p;
    EXPECT_THROW(p.parse("/nonexistent/path.txt"), std::runtime_error);
}

// ── NetworkBuilder ───────────────────────────────────────────────────────────

TEST(NetworkBuilderTest, MSTHasNMinusOneEdges) {
    std::vector<std::vector<int>> dist = {
        {0, 16, 45, 32},
        {16,  0, 18, 21},
        {45, 18,  0,  7},
        {32, 21,  7,  0}
    };
    NetworkBuilder nb;
    auto mst = nb.buildMST(4, dist);
    EXPECT_EQ(static_cast<int>(mst.size()), 3);
}

TEST(NetworkBuilderTest, MSTPicksMinimumWeight) {
    // A-B=1, B-C=2, A-C=10 → MST = A-B + B-C, peso total = 3
    std::vector<std::vector<int>> dist = {
        { 0,  1, 10},
        { 1,  0,  2},
        {10,  2,  0}
    };
    NetworkBuilder nb;
    auto mst = nb.buildMST(3, dist);
    int total = 0;
    for (auto& e : mst) total += e.weight;
    EXPECT_EQ(total, 3);
}

TEST(NetworkBuilderTest, FormatResultNotEmpty) {
    std::vector<std::vector<int>> dist = {{0, 7}, {7, 0}};
    NetworkBuilder nb;
    auto mst = nb.buildMST(2, dist);
    EXPECT_FALSE(nb.formatResult(mst).empty());
}

TEST(NetworkBuilderTest, FormatUsesLetterLabels) {
    std::vector<std::vector<int>> dist = {{0, 5}, {5, 0}};
    NetworkBuilder nb;
    auto mst = nb.buildMST(2, dist);
    auto result = nb.formatResult(mst);
    EXPECT_NE(result.find('A'), std::string::npos);
    EXPECT_NE(result.find('B'), std::string::npos);
}

// ── RoutingAndFlow ───────────────────────────────────────────────────────────

TEST(RoutingAndFlowTest, TSPReturnsFullTour) {
    std::vector<std::vector<int>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35,  0, 30},
        {20, 25, 30,  0}
    };
    RoutingAndFlow rf;
    auto tour = rf.SolveTSP(4, dist);
    EXPECT_EQ(static_cast<int>(tour.size()), 4);
}

TEST(RoutingAndFlowTest, MaxFlowPositive) {
    // Grafo simple: 0→1 cap 10, 1→2 cap 5
    std::vector<std::vector<int>> cap = {
        {0, 10,  0},
        {0,  0,  5},
        {0,  0,  0}
    };
    RoutingAndFlow rf;
    int flow = rf.MaxFlow(3, cap, 0, 2);
    EXPECT_EQ(flow, 5);
}

TEST(RoutingAndFlowTest, MaxFlowZeroWhenNoPath) {
    std::vector<std::vector<int>> cap = {
        {0, 0},
        {0, 0}
    };
    RoutingAndFlow rf;
    int flow = rf.MaxFlow(2, cap, 0, 1);
    EXPECT_EQ(flow, 0);
}

TEST(RoutingAndFlowTest, FormatTSPNotEmpty) {
    RoutingAndFlow rf;
    auto result = rf.FormatTSP({0, 1, 2});
    EXPECT_FALSE(result.empty());
}

TEST(RoutingAndFlowTest, FormatFlowContainsNumber) {
    RoutingAndFlow rf;
    auto result = rf.FormatFlow(42);
    EXPECT_NE(result.find("42"), std::string::npos);
}

// ── GeoDistricts ─────────────────────────────────────────────────────────────

TEST(GeoDistrictsTest, NearestNeighborReturnsValidIndex) {
    std::vector<std::pair<int,int>> centrals = {{0,0}, {10,10}, {20,0}};
    GeoDistricts geo;
    int idx = geo.nearestNeighbor(centrals, {1, 1});
    EXPECT_GE(idx, 0);
    EXPECT_LT(idx, static_cast<int>(centrals.size()));
}

TEST(GeoDistrictsTest, NearestNeighborFindsClosest) {
    std::vector<std::pair<int,int>> centrals = {{0,0}, {100,100}};
    GeoDistricts geo;
    // El punto (1,1) debe ser más cercano a (0,0)
    EXPECT_EQ(geo.nearestNeighbor(centrals, {1, 1}), 0);
}

TEST(GeoDistrictsTest, FormatResultNotEmpty) {
    std::vector<std::pair<int,int>> pts = {{0,0}, {10,10}};
    GeoDistricts geo;
    auto result = geo.formatResult(pts, pts);
    EXPECT_FALSE(result.empty());
}

TEST(GeoDistrictsTest, NearestNeighborEmptyCentralsReturnsMinusOne) {
    std::vector<std::pair<int,int>> empty;
    GeoDistricts geo;
    EXPECT_EQ(geo.nearestNeighbor(empty, {0, 0}), -1);
}

TEST(GeoDistrictsTest, NearestNeighborWithExcludeIndex) {
    // (0,0) is closest to query (1,1), but exclude it → should return index 1
    std::vector<std::pair<int,int>> centrals = {{0,0}, {5,5}};
    GeoDistricts geo;
    EXPECT_EQ(geo.nearestNeighbor(centrals, {1, 1}, 0), 1);
}

TEST(GeoDistrictsTest, NearestNeighborSingleCentral) {
    std::vector<std::pair<int,int>> centrals = {{3,4}};
    GeoDistricts geo;
    EXPECT_EQ(geo.nearestNeighbor(centrals, {0, 0}), 0);
}

TEST(GeoDistrictsTest, FormatResultEmptyCoordsReturnsEmpty) {
    std::vector<std::pair<int,int>> empty;
    GeoDistricts geo;
    EXPECT_TRUE(geo.formatResult(empty, empty).empty());
}

TEST(GeoDistrictsTest, FormatResultEmptyCentralsReturnsEmpty) {
    std::vector<std::pair<int,int>> pts = {{0,0}};
    std::vector<std::pair<int,int>> empty;
    GeoDistricts geo;
    EXPECT_TRUE(geo.formatResult(pts, empty).empty());
}

TEST(GeoDistrictsTest, FormatResultContainsLabels) {
    std::vector<std::pair<int,int>> pts = {{0,0}, {10,0}, {0,10}};
    GeoDistricts geo;
    auto result = geo.formatResult(pts, pts);
    EXPECT_NE(result.find('A'), std::string::npos);
    EXPECT_NE(result.find('B'), std::string::npos);
}

TEST(GeoDistrictsTest, VoronoiPolygonsEmptyInput) {
    std::vector<std::pair<int,int>> empty;
    GeoDistricts geo;
    EXPECT_TRUE(geo.voronoiPolygons(empty).empty());
}

TEST(GeoDistrictsTest, VoronoiPolygonsSizeMatchesInput) {
    std::vector<std::pair<int,int>> pts = {{0,0}, {10,0}, {5,8}};
    GeoDistricts geo;
    auto regions = geo.voronoiPolygons(pts);
    EXPECT_EQ(static_cast<int>(regions.size()), 3);
}

TEST(GeoDistrictsTest, VoronoiPolygonsSinglePoint) {
    std::vector<std::pair<int,int>> pts = {{5,5}};
    GeoDistricts geo;
    auto regions = geo.voronoiPolygons(pts);
    EXPECT_EQ(static_cast<int>(regions.size()), 1);
    EXPECT_FALSE(regions[0].empty());
}

// ── NetworkBuilder extra ──────────────────────────────────────────────────────

TEST(NetworkBuilderTest, MSTEmptyGraph) {
    NetworkBuilder nb;
    auto mst = nb.buildMST(0, {});
    EXPECT_TRUE(mst.empty());
}

TEST(NetworkBuilderTest, MSTSingleNode) {
    NetworkBuilder nb;
    std::vector<std::vector<int>> dist = {{0}};
    auto mst = nb.buildMST(1, dist);
    EXPECT_TRUE(mst.empty());
}

TEST(NetworkBuilderTest, MSTDisconnectedGraph) {
    // Two nodes with zero weight (no edge) → MST empty
    NetworkBuilder nb;
    std::vector<std::vector<int>> dist = {{0, 0}, {0, 0}};
    auto mst = nb.buildMST(2, dist);
    EXPECT_TRUE(mst.empty());
}

TEST(NetworkBuilderTest, FormatResultEmptyMST) {
    NetworkBuilder nb;
    std::vector<NetworkBuilder::Edge> empty;
    EXPECT_TRUE(nb.formatResult(empty).empty());
}

TEST(NetworkBuilderTest, MSTFiveNodes) {
    std::vector<std::vector<int>> dist = {
        { 0, 16, 45, 32,  0},
        {16,  0, 18, 21,  0},
        {45, 18,  0,  7,  0},
        {32, 21,  7,  0,  0},
        { 0,  0,  0,  0,  0}
    };
    NetworkBuilder nb;
    auto mst = nb.buildMST(5, dist);
    EXPECT_LE(static_cast<int>(mst.size()), 4);
}

// ── RoutingAndFlow extra ──────────────────────────────────────────────────────

TEST(RoutingAndFlowTest, TSPSingleNode) {
    std::vector<std::vector<int>> dist = {{0}};
    RoutingAndFlow rf;
    auto tour = rf.SolveTSP(1, dist);
    EXPECT_EQ(static_cast<int>(tour.size()), 1);
}

TEST(RoutingAndFlowTest, TSPTwoNodes) {
    std::vector<std::vector<int>> dist = {{0, 5}, {5, 0}};
    RoutingAndFlow rf;
    auto tour = rf.SolveTSP(2, dist);
    EXPECT_EQ(static_cast<int>(tour.size()), 2);
}

TEST(RoutingAndFlowTest, TSPStartsAtZero) {
    std::vector<std::vector<int>> dist = {
        {0, 10, 15},
        {10, 0, 35},
        {15, 35, 0}
    };
    RoutingAndFlow rf;
    auto tour = rf.SolveTSP(3, dist);
    EXPECT_EQ(tour[0], 0);
}

TEST(RoutingAndFlowTest, MaxFlowMultiplePaths) {
    // 0→1 cap 10, 0→2 cap 10, 1→3 cap 10, 2→3 cap 10 → max flow = 20
    std::vector<std::vector<int>> cap = {
        {0, 10, 10,  0},
        {0,  0,  0, 10},
        {0,  0,  0, 10},
        {0,  0,  0,  0}
    };
    RoutingAndFlow rf;
    EXPECT_EQ(rf.MaxFlow(4, cap, 0, 3), 20);
}

TEST(RoutingAndFlowTest, MaxFlowBottleneck) {
    // 0→1 cap 100, 1→2 cap 1 → bottleneck = 1
    std::vector<std::vector<int>> cap = {
        {0, 100,   0},
        {0,   0,   1},
        {0,   0,   0}
    };
    RoutingAndFlow rf;
    EXPECT_EQ(rf.MaxFlow(3, cap, 0, 2), 1);
}

TEST(RoutingAndFlowTest, MaxFlowSourceEqualsNMinusOne) {
    // Standard 4-node problem
    std::vector<std::vector<int>> cap = {
        {0, 16, 13,  0},
        {0,  0, 10, 12},
        {0,  4,  0, 14},
        {0,  0,  0,  0}
    };
    RoutingAndFlow rf;
    int flow = rf.MaxFlow(4, cap, 0, 3);
    EXPECT_GT(flow, 0);
}

TEST(RoutingAndFlowTest, FormatTSPEmptyTour) {
    RoutingAndFlow rf;
    auto result = rf.FormatTSP({});
    EXPECT_FALSE(result.empty());
}

TEST(RoutingAndFlowTest, FormatTSPContainsArrow) {
    RoutingAndFlow rf;
    auto result = rf.FormatTSP({0, 1, 2});
    EXPECT_NE(result.find("->"), std::string::npos);
}

TEST(RoutingAndFlowTest, FormatFlowZero) {
    RoutingAndFlow rf;
    auto result = rf.FormatFlow(0);
    EXPECT_NE(result.find("0"), std::string::npos);
}

// ── GraphParser extra ─────────────────────────────────────────────────────────

TEST(GraphParserTest, ParsesFourByFour) {
    auto path = writeTempInput(
        "4\n"
        "0 16 45 32\n16 0 18 21\n45 18 0 7\n32 21 7 0\n"
        "0 16 45 32\n16 0 18 21\n45 18 0 7\n32 21 7 0\n"
        "(0,0)\n(1,1)\n(2,2)\n(3,3)\n"
    );
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.N, 4);
    EXPECT_EQ(p.dist[0][1], 16);
    EXPECT_EQ(p.capacity[2][3], 7);
    EXPECT_EQ(p.coords[3].first, 3);
    EXPECT_EQ(p.coords[3].second, 3);
}

TEST(GraphParserTest, ParsesNegativeCoords) {
    auto path = writeTempInput("1\n0\n0\n(-5,-10)\n");
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.coords[0].first, -5);
    EXPECT_EQ(p.coords[0].second, -10);
}

TEST(GraphParserTest, ParsesZeroNodes) {
    auto path = writeTempInput("0\n");
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.N, 0);
    EXPECT_TRUE(p.dist.empty());
    EXPECT_TRUE(p.capacity.empty());
    EXPECT_TRUE(p.coords.empty());
}

TEST(GraphParserTest, ParsesThreeByThree) {
    auto path = writeTempInput(
        "3\n"
        "0 5 10\n5 0 3\n10 3 0\n"
        "0 2 4\n2 0 1\n4 1 0\n"
        "(0,0)\n(5,0)\n(2,4)\n"
    );
    GraphParser p;
    p.parse(path);
    EXPECT_EQ(p.N, 3);
    EXPECT_EQ(p.dist[1][2], 3);
    EXPECT_EQ(p.capacity[0][2], 4);
    EXPECT_EQ(p.coords[2].first, 2);
    EXPECT_EQ(p.coords[2].second, 4);
}

// ── RoutingAndFlow extra coverage ────────────────────────────────────────────

TEST(RoutingAndFlowTest, MaxFlowLargerGraph) {
    // Red con multiples caminos y bottlenecks
    std::vector<std::vector<int>> cap = {
        {0, 10,  0,  0,  0},
        {0,  0,  5,  8,  0},
        {0,  0,  0,  0,  7},
        {0,  0,  3,  0,  6},
        {0,  0,  0,  0,  0}
    };
    RoutingAndFlow rf;
    int flow = rf.MaxFlow(5, cap, 0, 4);
    EXPECT_GT(flow, 0);
    EXPECT_LE(flow, 10);
}

TEST(RoutingAndFlowTest, TSPThreeNodes) {
    std::vector<std::vector<int>> dist = {
        { 0, 10, 15},
        {10,  0, 35},
        {15, 35,  0}
    };
    RoutingAndFlow rf;
    auto tour = rf.SolveTSP(3, dist);
    EXPECT_EQ(static_cast<int>(tour.size()), 3);
    EXPECT_EQ(tour[0], 0);
}

TEST(RoutingAndFlowTest, FormatTSPSingleNode) {
    RoutingAndFlow rf;
    auto result = rf.FormatTSP({0});
    EXPECT_NE(result.find('A'), std::string::npos);
}

TEST(GeoDistrictsTest, VoronoiPolygonsTwoPoints) {
    std::vector<std::pair<int,int>> pts = {{0,0}, {10,0}};
    GeoDistricts geo;
    auto regions = geo.voronoiPolygons(pts);
    EXPECT_EQ(static_cast<int>(regions.size()), 2);
}
