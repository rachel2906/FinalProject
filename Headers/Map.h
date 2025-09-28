#ifndef MAP_H
#define MAP_H

#include <QString>
#include <QPoint>
#include <vector>

// Cấu trúc đại diện 1 điểm (node) trong bản đồ
struct Node {
    int id;             // mã số node
    QString name;       // tên node (ví dụ: thư viện, căn tin)
    QPoint position;    // tọa độ hiển thị trên bản đồ
};

// Cấu trúc đại diện 1 cạnh (edge) nối 2 node
struct Edge {
    int from;           // node bắt đầu
    int to;             // node kết thúc
    double distance;    // khoảng cách
};

class Map {
public:
    Map(const QString& name);

    void addNode(int id, const QString& name, const QPoint& pos);
    void addEdge(int from, int to, double distance);

    const std::vector<Node>& getNodes() const;
    const std::vector<Edge>& getEdges() const;

private:
    QString name;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};

#endif // MAP_H
