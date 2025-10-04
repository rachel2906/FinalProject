#include "../Headers/UTEpath.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QString>

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

QString UTEPath::findNearestLocationName(double x, double y) {
    // 1. Tìm Node ID gần nhất
    int nearestNodeId = graph.findNearestNodeId(x, y);

    if (nearestNodeId == -1) {
        return ""; // Không tìm thấy node nào
    }

    // 2. Dùng Node ID tra cứu ngược lại trong mapping.txt
    // Duyệt qua mapping để tìm xem node_id này thuộc địa điểm nào
    for (const auto& pair : locationToNodes) {
        const std::string& locationName = pair.first;
        const std::vector<int>& nodeIds = pair.second;

        // Kiểm tra xem nearestNodeId có nằm trong danh sách nodeIds của địa điểm này không
        if (std::find(nodeIds.begin(), nodeIds.end(), nearestNodeId) != nodeIds.end()) {
            // Đã tìm thấy tên địa điểm
            return QString::fromStdString(locationName);
        }
    }

    // Trường hợp node gần nhất không được ánh xạ (map) thành tên địa điểm nào
    return "Vị trí đã chọn (" + QString::number(nearestNodeId) + ")"; 
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
