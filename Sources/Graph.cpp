#include "../Headers/Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

using namespace std;

// ----------------- Node -----------------
void Graph::addNode(int id, double x, double y, const string& name) {
    nodes[id] = Node(id, x, y, name);
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
    adj[from].push_back({to, weight});
    adj[to].push_back({from, weight});
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
    return sqrt(dx * dx + dy * dy);
}

// ----------------- Load from file -----------------
bool Graph::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return false;
    }

    string line;
    regex pattern(R"(^\s*(\d+)\s+(\d+)\s+(\d+)\s+\"([^\"]+)\"\s+\{([0-9,\s]*)\}\s*$)");
    smatch match;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        if (!regex_match(line, match, pattern)) {
            cerr << "Warning: Invalid line format -> " << line << endl;
            continue;
        }

        int id = stoi(match[1]);
        double x = stod(match[2]);
        double y = stod(match[3]);
        string name = match[4];
        string neighbors = match[5];

        // Thêm node
        addNode(id, x, y, name);

        // Parse danh sách kề
        stringstream ss(neighbors);
        string token;
        while (getline(ss, token, ',')) {
            if (token.empty()) continue;
            int neighborId = stoi(token);
            // Nếu neighbor đã tồn tại thì thêm cạnh vô hướng
            if (hasNode(neighborId)) {
                double dist = calcDistance(getNode(id), getNode(neighborId));
                addUndirectedEdge(id, neighborId, dist);
            }
        }
    }

    fin.close();
    return true;
}

