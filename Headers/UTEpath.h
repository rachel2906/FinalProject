#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Graph.h"
#include "Algorithm.h"
using namespace std;

class UTEPath {
public:
    static unordered_map<string, vector<int>> locationToNodes; // ánh xạ địa điểm -> danh sách node
    static Graph graph; // đồ thị toàn trường

    static void loadMapping(const string& filename = "../Resources/mapping.txt");
    static vector<int> findPath(const string& startLocation, const string& endLocation);
};
