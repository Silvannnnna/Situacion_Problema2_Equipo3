#include "../include/GraphParser.h"
#include "../include/NetworkBuilder.h"
#include "../include/RoutingAndFlow.h"
#include "../include/GeoDistricts.h"

#include <iostream>

int main() {
    GraphParser parser;
    parser.parse("data/input.txt");

    NetworkBuilder network;
    auto mst = network.buildMST(parser.N, parser.dist);
    std::cout << "1. Forma de cablear las colonias:\n";
    std::cout << network.formatResult(mst) << "\n";

    RoutingAndFlow routing;
    auto tour = routing.solveTSP(parser.N, parser.dist);
    std::cout << "2. Ruta de correspondencia:\n";
    std::cout << routing.formatTSP(tour) << "\n";

    int flow = routing.maxFlow(parser.N, parser.capacity, 0, parser.N - 1);
    std::cout << "3. Flujo maximo:\n";
    std::cout << routing.formatFlow(flow) << "\n";

    GeoDistricts geo;
    std::cout << "4. Central mas cercana:\n";
    std::cout << geo.formatResult(parser.coords, parser.coords) << "\n";

    return 0;
}
