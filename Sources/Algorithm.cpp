
#include "../Headers/Algorithm.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

vector<int> Algorithm::dijkstra(
    const Graph& graph,
    const vector<int>& startNodes,
    const vector<int>& endNodes
) {
    const auto& nodes = graph.getAllNodes();
    const double INF = numeric_limits<double>::infinity();

    unordered_map<int, double> dist;
    unordered_map<int, int> prev;

    // Khởi tạo khoảng cách
    for (const auto& kv : nodes) {
        dist[kv.first] = INF;
        prev[kv.first] = -1;
    }

    using P = pair<double, int>;
    priority_queue<P, vector<P>, greater<P>> pq;

    // Multi-source
    for (int s : startNodes) {
        if (nodes.find(s) != nodes.end()) {
            dist[s] = 0;
            pq.push({0, s});
        }
    }

    int reachedEnd = -1;
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (find(endNodes.begin(), endNodes.end(), u) != endNodes.end()) {
            reachedEnd = u;
            break;
        }

        if (d > dist[u]) continue;

        for (auto& [v, w] : graph.getNeighbors(u)) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Truy vết đường đi
    vector<int> path;
    if (reachedEnd != -1) {
        for (int cur = reachedEnd; cur != -1; cur = prev[cur]) {
            path.push_back(cur);
        }
        reverse(path.begin(), path.end());
    }

    return path;
}
