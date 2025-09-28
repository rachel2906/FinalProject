#include "Map.h"

Map::Map(const QString& name) : name(name) {}

void Map::addNode(int id, const QString& name, const QPoint& pos) {
    nodes.push_back({id, name, pos});
}

void Map::addEdge(int from, int to, double distance) {
    edges.push_back({from, to, distance});
}

const std::vector<Node>& Map::getNodes() const {
    return nodes;
}

const std::vector<Edge>& Map::getEdges() const {
    return edges;
}
