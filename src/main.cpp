#include "graph.hpp"
#include "pathfinder.hpp"
#include "renderer.hpp"
#include <unordered_map>
#include <iomanip>

const int width = 1200;
const int height = 800;

struct NodePosition {
    Vector2 pos;
    Color color;
};

int main(){
    std::freopen("/dev/null", "w", stderr); // Silence MacOS
    SetTraceLogLevel(LOG_NONE); // Silence Raylib
    
    Renderer renderer(width, height, "Dijkstra-Pathfinder");
    renderer.LoadMap("./maps/map.png");
    
    Graph graph("./maps/map_graph.json");
    auto G = graph.getGraph();
    
    
    // Load bounds
    std::ifstream f("./maps/map_bounds.json");
    json bounds; f >> bounds; f.close();
    
    // Extract bounds with verification
    double lat_max = bounds["lat_max"].get<double>();
    double lat_min = bounds["lat_min"].get<double>();
    double lon_max = bounds["lon_max"].get<double>();
    double lon_min = bounds["lon_min"].get<double>();
    
    // Get actual map dimensions
    double actual_map_width = static_cast<double>(renderer.MapWidth());
    double actual_map_height = static_cast<double>(renderer.MapHeight());
    
    // Debug output
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Bounds: lat(" << lat_min << ", " << lat_max << "), ";
    std::cout << "lon(" << lon_min << ", " << lon_max << ")" << std::endl;
    std::cout << "Map size: " << actual_map_width << "x" << actual_map_height << std::endl;
    
    // Check if we have image dimensions from Python script
    double expected_width = actual_map_width;
    double expected_height = actual_map_height;
    
    if (bounds.contains("image_width") && bounds.contains("image_height")) {
        expected_width = bounds["image_width"].get<double>();
        expected_height = bounds["image_height"].get<double>();
        
        std::cout << "Expected size from Python: " << expected_width << "x" << expected_height << std::endl;
        
        if (std::abs(actual_map_width - expected_width) > 1.0 || 
        std::abs(actual_map_height - expected_height) > 1.0) {
            std::cout << "WARNING: Image size mismatch detected!" << std::endl;
        }
    }
    
    // Calculate coordinate ranges
    double lat_range = lat_max - lat_min;
    double lon_range = lon_max - lon_min;
    
    std::cout << "Coordinate ranges: lat=" << lat_range << ", lon=" << lon_range << std::endl;
    
    // Pre-calculate all node positions using the EXACT same formula as Python
    std::unordered_map<std::string, NodePosition> nodePositions;
    
    
    double lat1, lon1, lat2, lon2;
    std::cout << "Enter the start position (latitude, longitude): ";
    std::cin >> lat1 >> lon1;
    std::cout << "Enter the end position (latitude, longitude): ";
    std::cin >> lat2 >> lon2;

    string start_node, end_node;
    double min_distance1 = std::numeric_limits<double>::max();
    double min_distance2 = std::numeric_limits<double>::max();

    for (const auto& [id, node]: G){
        double distance1 = pow(node.lat - lat1, 2) + pow(node.lon - lon1, 2);

        if (distance1 < min_distance1){
            min_distance1 = distance1;
            start_node = id;
        }

        double distance2 = pow(node.lat - lat2, 2) + pow(node.lon - lon2, 2);
        if (distance2 < min_distance2){
            min_distance2 = distance2;
            end_node = id;
        }
    }

    
    Pathfinder dijkstra(G, start_node, end_node);
    const auto& dist = dijkstra.GetDistances();

    if (dist.at(end_node) == std::numeric_limits<double>::infinity()){
        std::cout << "No path from " << start_node << " to " << end_node << " exists." << std::endl;
        return 1;
    }
    
    for (const auto& [id, node] : G) {
        // Use EXACT same calculation as Python script
        double x = ((node.lon - lon_min) / lon_range) * actual_map_width;
        double y = ((lat_max - node.lat) / lat_range) * actual_map_height;
        
        Color nodeColor = GREEN;
        if (id == start_node) {
            nodeColor = BLUE;
        } else if (id == end_node) {
            nodeColor = RED;
        }
        
        nodePositions[id] = {
            {static_cast<float>(x), static_cast<float>(y)}, 
            nodeColor
        };
    }
    
    // Pre-calculate path positions
    std::vector<Vector2> pathPositions;
    auto path = dijkstra.GetPath();
    if (path.size() > 1) {
        for (const auto& nodeId : path) {
            pathPositions.push_back(nodePositions[nodeId].pos);
        }
    }

    while (renderer.Running()){
        renderer.HandleInput();

        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(renderer.GetCamera());
        renderer.DrawMap();

        // Draw all nodes
        for (const auto& [id, nodePos] : nodePositions) {
            DrawCircleV(nodePos.pos, 18.0f, nodePos.color);
            
            // Draw a small cross in the center
            DrawLineEx({nodePos.pos.x - 4, nodePos.pos.y}, 
                      {nodePos.pos.x + 4, nodePos.pos.y}, 2.0f, BLACK);
            DrawLineEx({nodePos.pos.x, nodePos.pos.y - 4}, 
                      {nodePos.pos.x, nodePos.pos.y + 4}, 2.0f, BLACK);
        }

        // Draw the path
        if (pathPositions.size() > 1) {
            for (size_t i = 0; i < pathPositions.size() - 1; i++) {
                DrawLineEx(pathPositions[i], pathPositions[i + 1], 20.0f, ORANGE);
            }
        }

        EndMode2D();
        EndDrawing();
    }
    
    return 0;
}