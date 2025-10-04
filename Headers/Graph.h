#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <cmath>
using namespace std;

struct Node {
    int id;
    double x, y;

    Node(int _id = -1, double _x = 0, double _y = 0)
        : id(_id), x(_x), y(_y) {}
};

class Graph {
private:
    unordered_map<int, Node> nodes;                  // id -> Node
    unordered_map<int, vector<pair<int, double>>> adj; // id -> { (neighbor_id, weight), ... }

public:
    // constructor: in ra "Graph created"
    Graph() {
         cout << "Graph is creating" << endl;
    }   

    // ---- Node ----
    void addNode(int id, double x, double y);
    bool hasNode(int id) const;
    const Node& getNode(int id) const;
    const unordered_map<int, Node>& getAllNodes() const;

    // ---- Edge ----
    void addEdge(int from, int to, double weight);
    void addUndirectedEdge(int from, int to, double weight);
    const vector<pair<int,double>>& getNeighbors(int id) const;

    // ---- Load from file ----
    bool loadFromFile(const string& filename = "../Resources/nodes.txt");

    // ---- Getter toàn bộ ----
    const unordered_map<int, vector<pair<int,double>>>& getAdjList() const { return adj; }
    const unordered_map<int, Node>& getNodes() const { return nodes; }

    // ---- Tính toán ----
    static double calcDistance(const Node& a, const Node& b);

    int findNearestNodeId(double x, double y) const; //Tìm ID của node gần nhất với tọa độ (x, y)
};
