#include "../Headers/UTEPath.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Khai báo static member
Graph UTEPath::graph; 
unordered_map<string, vector<int>> UTEPath::locationToNodes;

// --- Static initializer để build graph ngay ---
struct GraphInitializer {
    GraphInitializer() {
        UTEPath::graph.loadFromFile("../Resources/nodes.txt");
        UTEPath::loadMapping("../Resources/mapping.txt");
    }
};
// đối tượng tĩnh, sẽ chạy khi chương trình bắt đầu
static GraphInitializer graphInit;

// ---------------- Load mapping ----------------
void UTEPath::loadMapping(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) return;

    string line;
    while (getline(fin, line)) {
        // bỏ dấu " ở đầu và lấy location
        size_t firstQuote = line.find('"');
        size_t secondQuote = line.find('"', firstQuote + 1);
        string location = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);

        string rest = line.substr(secondQuote + 1);
        stringstream ss(rest);
        int nodeId;
        while (ss >> nodeId) locationToNodes[location].push_back(nodeId);
    }
}

// ---------------- Find Path ----------------
vector<int> UTEPath::findPath(const string& startLocation, const string& endLocation) {
    auto itStart = locationToNodes.find(startLocation);
    auto itEnd = locationToNodes.find(endLocation);

    if (itStart == locationToNodes.end() || itEnd == locationToNodes.end()) return {};

    const vector<int>& startNodes = itStart->second;
    const vector<int>& endNodes = itEnd->second;

    vector<int> path = Algorithm::dijkstra(graph, startNodes, endNodes);
    return path;
}
