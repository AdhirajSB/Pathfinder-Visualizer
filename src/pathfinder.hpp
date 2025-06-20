#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include "graph.hpp"

template<typename Key, typename Value>
using umap = std::unordered_map<Key, Value>;
using string = std::string;


class Pathfinder {
    private:
    string m_StartNode, m_EndNode;
    umap<string, double> m_Dist;
    umap<string, string> m_Prev;
    umap<string, Node> m_Graph;

    public:
    Pathfinder(const umap<string, Node>& graph, const string& startNode, const string& endNode)
        : m_StartNode(startNode), m_EndNode(endNode), m_Graph(graph) 
    {
        if (m_Graph.find(m_StartNode) == m_Graph.end()){
            throw std::runtime_error("Start node not found in graph\n");
        }
        // Can switch to other algorithms later
        dijkstra();
    }

    inline const umap<string, double> GetDistances() const { return m_Dist; }
    inline std::vector<string> GetPath() const {
        std::vector<string> path;
        for (string at = m_EndNode; !at.empty(); at = m_Prev.at(at)) {
            path.push_back(at);
            if (at == m_StartNode) break;
        }
        std::reverse(path.begin(), path.end());
        return path; 
    }

    private:
    void dijkstra() {
        for (const auto& [id, _]: m_Graph)
            m_Dist[id] = std::numeric_limits<double>::infinity();
        

        using Pair = std::pair<double, string>;
        std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;

        m_Dist[m_StartNode] = 0;
        pq.emplace(0, m_StartNode);

        while (!pq.empty()){
            auto [cost, u] = pq.top(); pq.pop();
            
            const auto& neighbours = m_Graph[u].neighbours;
            for (auto& [v, weight]: neighbours) {
                double alt = m_Dist[u] + weight;
                if (alt < m_Dist[v]){
                    m_Dist[v] = alt;
                    m_Prev[v] = u;
                    pq.emplace(alt, v);
                }
            }
        }
    }
};

#endif