#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json; 

struct Node {
    double lat, lon;
    std::vector<std::pair<std::string, double>> neighbours; // (Neighbor ID, weight)
};


class Graph {
    private:
    std::unordered_map<std::string, Node> m_Graph;

    public:
    Graph(const std::string& filePath){
        std::ifstream f(filePath);
        if (!f.is_open()) {
            throw std::runtime_error("Failed to open the file\n");
        }

        json data; f >> data;

        for (const auto& [id, pos]: data["nodes"].items()) {
            m_Graph[id].lat = pos["lat"];
            m_Graph[id].lon = pos["lon"];
        }

        for (const auto& edge: data["edges"]){
            std::string from = edge["from"];
            std::string to = edge["to"];
            double weight = edge["weight"];

            m_Graph[from].neighbours.emplace_back(to, weight);
            m_Graph[to].neighbours.emplace_back(from, weight);
        }
    }

    inline std::unordered_map<std::string, Node> getGraph() { return m_Graph; }
};

#endif