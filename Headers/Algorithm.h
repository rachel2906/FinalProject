#pragma once
#include <vector>
#include "Graph.h"
using namespace std;

class Algorithm {
public:
    // Dijkstra: tìm đường đi ngắn nhất giữa 2 tập node
    static vector<int> dijkstra(
        const Graph& graph,
        const vector<int>& startNodes,
        const vector<int>& endNodes
    ); // trả về danh sách id node trên đường đi ngắn nhất
    // vector <int> :  0 1 2 4
};
