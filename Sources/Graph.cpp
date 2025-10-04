#include "../Headers/Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <cmath>
using namespace std;

// ----------------- Node -----------------
void Graph::addNode(int id, double x, double y) {
    nodes[id] = Node(id, x, y);
}

bool Graph::hasNode(int id) const {
    return nodes.find(id) != nodes.end();
}

const Node& Graph::getNode(int id) const {
    return nodes.at(id);
}

const unordered_map<int, Node>& Graph::getAllNodes() const {
    return nodes;
}

// ----------------- Edge -----------------
void Graph::addEdge(int from, int to, double weight) {
    adj[from].push_back({to, weight});
}

void Graph::addUndirectedEdge(int from, int to, double weight) {
    if (from == to) return;
    auto &fromList = adj[from];
    bool found = false;
    for (auto &p : fromList) if (p.first == to) { found = true; break; }
    if (!found) fromList.emplace_back(to, weight);

    auto &toList = adj[to];
    found = false;
    for (auto &p : toList) if (p.first == from) { found = true; break; }
    if (!found) toList.emplace_back(from, weight);
}

const vector<pair<int,double>>& Graph::getNeighbors(int id) const {
    static vector<pair<int,double>> empty;
    auto it = adj.find(id);
    return (it != adj.end()) ? it->second : empty;
}

// ----------------- Distance -----------------
double Graph::calcDistance(const Node& a, const Node& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

// ----------------- Load from file (numbers only, neighbors space-separated) -----------------
bool Graph::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) return false;

    unordered_map<int, vector<int>> pendingNeighbors;
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        int id;
        double x, y;
        char ch;

        // đọc id, x, y
        if (!(ss >> id >> x >> y)) continue;

        // đọc '{'
        if (!(ss >> ch) || ch != '{') continue;

        // đọc neighbor ids
        vector<int> neighbors;
        int nid;
        while (ss >> nid) {
            neighbors.push_back(nid);
        }

        // thêm node
        addNode(id, x, y);

        // lưu tạm neighbors
        pendingNeighbors[id] = neighbors;
    }
    fin.close();

    // add undirected edges
    for (const auto &kv : pendingNeighbors) {
        int id = kv.first;
        for (int neighborId : kv.second) {
            if (id < neighborId && hasNode(neighborId)) {
                double dist = calcDistance(getNode(id), getNode(neighborId));
                addUndirectedEdge(id, neighborId, dist);
            }
        }
    }

   // in các node đã load 
   for (const auto& [id, node] : nodes) {
       cout << "Loaded Node " << id << " at (" << node.x << ", " << node.y << ")\n";
   }

    return true;
}

int Graph::findNearestNodeId(double x, double y) const {
    double minDistance = std::numeric_limits<double>::max();
    int nearestId = -1;

    for (const auto& pair : nodes) {
        int id = pair.first;
        const Node& node = pair.second;

        // Tính khoảng cách Euclid
        double dx = x - node.x;
        double dy = y - node.y;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance < minDistance) {
            minDistance = distance;
            nearestId = id;
        }
    }

    return nearestId;
}

