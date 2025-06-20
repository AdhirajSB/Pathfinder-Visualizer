#include "graph.hpp"
#include "pathfinder.hpp"
#include "renderer.hpp"

const int width = 800;
const int height = 600;

int main(){
    Renderer renderer(width, height, "Dijkstra-Pathfinder");
    renderer.LoadMap("./maps/map.png");
    
    Graph graph("./maps/map_graph.json");
    auto G = graph.getGraph();
    
    string start_node = G.begin()->first;
    string end_node = next(G.begin(), 3)->first;
    
    Pathfinder dijkstra(G, start_node, end_node);
    const auto& dist = dijkstra.GetDistances();

    if (dist.at(end_node) == std::numeric_limits<double>::infinity()){
        std::cout << "No path from " << start_node << " to " << end_node << " exists." << std::endl;
        return 1;
    }

    while (renderer.Running()){
        renderer.HandleInput();
        renderer.DrawBackground();
    }
}