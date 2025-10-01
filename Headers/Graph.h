
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <cmath>
using namespace std;

struct Node {
    int id;
    double x, y;
    string name;

    Node(int _id = -1, double _x = 0, double _y = 0, const string& _name = "-")
        : id(_id), x(_x), y(_y), name(_name) {}
};

class Graph {
private:
    // Danh sách node: id -> Node
    unordered_map<int, Node> nodes; //nodes[id] = Node id,x,y,name
    

    // Danh sách kề: id -> vector< (id kề, trọng số) >
    unordered_map<int, vector<pair<int, double>>> adj; //adj[id] = { (id_kề, trọng số), ... }

public:
    Graph() = default;

    // ---- Quản lý Node ----
    void addNode(int id, double x, double y, const string& name = "-");
    bool hasNode(int id) const;
    const Node& getNode(int id) const;
    const unordered_map<int, Node>& getAllNodes() const;

    // ---- Quản lý Edge ----
    void addEdge(int from, int to, double weight);
    void addUndirectedEdge(int from, int to, double weight);

    const vector<pair<int,double>>& getNeighbors(int id) const;

    // ---- Xây dựng từ file ----
    bool loadFromFile(const string& filename = "../Resources/edges.txt");

    // ---- Getter toàn bộ ----
    const unordered_map<int, vector<pair<int,double>>>& getAdjList() const { return adj; }
    const unordered_map<int, Node>& getNodes() const { return nodes; }

    // ---- Tính toán ----
    static double calcDistance(const Node& a, const Node& b);
};
