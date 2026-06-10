#include "../include/GraphParser.h"
#include "../include/NetworkBuilder.h"
#include "../include/RoutingAndFlow.h"
#include "../include/GeoDistricts.h"

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <csignal>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using json = nlohmann::json;

// ── Modo consola ──────────────────────────────────────────────────────────────

static std::string format_voronoi(
    const std::vector<std::vector<std::pair<double,double>>>& polygons)
{
    std::ostringstream oss;
    for (size_t i = 0; i < polygons.size(); i++) {
        oss << "Poligono " << (i + 1) << ": ";
        for (const auto& pt : polygons[i]) {
            oss << std::fixed << std::setprecision(2)
                << "(" << pt.first << "," << pt.second << ") ";
        }
        if (i + 1 < polygons.size()) {
            oss << "\n";
        }
    }
    return oss.str();
}

static void run_console() {
    GraphParser parser;
    parser.parse("data/input.txt");

    NetworkBuilder network;
    auto mst = network.buildMST(parser.N, parser.dist);
    std::cout << "1. Forma de cablear las colonias:\n";
    std::cout << network.formatResult(mst) << "\n";

    RoutingAndFlow routing;
    auto tour = routing.SolveTSP(parser.N, parser.dist);
    std::cout << "2. Ruta de correspondencia:\n";
    std::cout << routing.FormatTSP(tour) << "\n";

    int flow = routing.MaxFlow(parser.N, parser.capacity, 0, parser.N - 1);
    std::cout << "3. Flujo maximo:\n";
    std::cout << routing.FormatFlow(flow) << "\n";

    GeoDistricts geo;
    auto polygons = geo.voronoiPolygons(parser.coords);
    std::cout << "4. Poligonos de Voronoi:\n";
    std::cout << format_voronoi(polygons) << "\n";
}

// ── Modo servidor ─────────────────────────────────────────────────────────────

static httplib::Server* g_svr = nullptr;

static void signal_handler(int) {
    if (g_svr != nullptr) {
        g_svr->stop();
    }
}

static std::string build_solve_response(const std::string& dataFile = "data/input.txt") {
    GraphParser parser;
    parser.parse(dataFile);

    NetworkBuilder network;
    auto mst = network.buildMST(parser.N, parser.dist);

    RoutingAndFlow routing;
    auto tour = routing.SolveTSP(parser.N, parser.dist);
    int flow = routing.MaxFlow(parser.N, parser.capacity, 0, parser.N - 1);

    GeoDistricts geo;

    json result;
    result["cabling"]        = network.formatResult(mst);
    result["route"]          = routing.FormatTSP(tour);
    result["maxFlow"]        = flow;
    result["nearestCentral"] = geo.formatResult(parser.coords, parser.coords);
    return result.dump();
}

static void run_server() {
    httplib::Server svr;
    g_svr = &svr;

    std::signal(SIGTERM, signal_handler);
    std::signal(SIGINT,  signal_handler);

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        json body;
        body["status"] = "UP";
        res.set_content(body.dump(), "application/json");
    });

    svr.Get("/solve", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string file = req.get_param_value("file");
            if (file.empty()) {
                file = "data/input.txt";
            }
            res.set_content(build_solve_response(file), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            json err;
            err["error"] = e.what();
            res.set_content(err.dump(), "application/json");
        }
    });

    std::cout << "Server running on port 8080\n";
    svr.listen("0.0.0.0", 8080);
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--server") {
        run_server();
    } else {
        run_console();
    }
    return 0;
}
