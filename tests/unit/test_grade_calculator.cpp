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
    auto tour = rf.solveTSP(4, dist);
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
    int flow = rf.maxFlow(3, cap, 0, 2);
    EXPECT_EQ(flow, 5);
}

TEST(RoutingAndFlowTest, MaxFlowZeroWhenNoPath) {
    std::vector<std::vector<int>> cap = {
        {0, 0},
        {0, 0}
    };
    RoutingAndFlow rf;
    int flow = rf.maxFlow(2, cap, 0, 1);
    EXPECT_EQ(flow, 0);
}

TEST(RoutingAndFlowTest, FormatTSPNotEmpty) {
    RoutingAndFlow rf;
    auto result = rf.formatTSP({0, 1, 2});
    EXPECT_FALSE(result.empty());
}

TEST(RoutingAndFlowTest, FormatFlowContainsNumber) {
    RoutingAndFlow rf;
    auto result = rf.formatFlow(42);
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
