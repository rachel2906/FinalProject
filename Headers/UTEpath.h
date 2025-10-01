#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Graph.h"
#include "Algorithm.h"
using namespace std;

class UTEPath {
private:
    // Ánh xạ tên địa điểm -> danh sách id node (các cửa ra vào)
    static unordered_map<string, vector<int>> locationToNodes;

    // Đồ thị toàn trường
    static Graph graph;

    // Hàm load dữ liệu ánh xạ từ mapping.txt
    static void loadMapping(const string& filename); //vector<int> startNodes, vector<int> endNodes                 

public:
    // Hàm static: chỉ cần gọi UTEPath::findPath("A", "B")
    static vector<int> findPath(const string& startLocation, const string& endLocation);
};
// vector <int> path = findPath("Cổng chính", "Thư viện");
    