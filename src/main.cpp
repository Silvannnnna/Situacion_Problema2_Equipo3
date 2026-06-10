#include "../include/GraphParser.h"
#include "../include/NetworkBuilder.h"
#include "../include/RoutingAndFlow.h"
#include "../include/GeoDistricts.h"

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

using json = nlohmann::json;

static std::string build_solve_response() {
    GraphParser parser;
    parser.parse("data/input.txt");

    NetworkBuilder network;
    auto mst = network.buildMST(parser.N, parser.dist);

    RoutingAndFlow routing;
    auto tour = routing.SolveTSP(parser.N, parser.dist);
    int flow = routing.maxFlow(parser.N, parser.capacity, 0, parser.N - 1);

    GeoDistricts geo;

    json result;
    result["cabling"] = network.formatResult(mst);
    result["route"] = routing.formatTSP(tour);
    result["maxFlow"] = flow;
    result["nearestCentral"] = geo.formatResult(parser.coords, parser.coords);
    return result.dump();
}

int main() {
    httplib::Server svr;

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        json body;
        body["status"] = "UP";
        res.set_content(body.dump(), "application/json");
    });

    svr.Get("/solve", [](const httplib::Request&, httplib::Response& res) {
        try {
            res.set_content(build_solve_response(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            json err;
            err["error"] = e.what();
            res.set_content(err.dump(), "application/json");
        }
    });

    std::cout << "Server running on port 8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}
