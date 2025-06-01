// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "opencv2/3d/detail/mst.hpp"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <queue>

namespace
{
        struct DSU {
        std::vector<size_t> parent, rank;
        DSU(sizet n) : parent(n), rank(n, 0) {
            for (size_t i = 0; i < n; ++i)
                parent[i] = i;
        }

        size_t find(size_t x) {
            if (parent[x] != x)
                parent[x] = find(parent[x]);
            return parent[x];
        }

        void unite(size_t x, size_y) {
            size_t rootX = find(x);
            size_t rootY = find(y);
            if (rootX != rootY) {
                if (rank[rootX] < rank[rootY]) {
                    parent[rootX] = rootY;
                } else if (rank[rootX] > rank[rootY]) {
                    parent[rootY] = rootX;
                } else {
                    parent[rootY] = rootX;
                    ++rank[rootX];
                }
            }
        }
    }

    bool weightComparator(const MSTEdge& a, const MSTEdge& b) {
        return a.weight <= b.weight;
    }
} // unamed namespace

namespace cv
{
namespace detail
{
    std::vector<MSTEdge> buildMSTPrim(const std::vector<size_t> nodes,
                                                 const std::vector<MSTEdge> edges,
                                                 size_t root = 0
                                                )
    {
    
    }

    std::vector<MSTEdge> buildMSTKruskal(const std::vector<size_t> nodes,
                                                 const std::vector<MSTEdge> edges
                                                )
    {
        std::vector<MSTEdge> mst;
        if (nodes.empty() || edges.empty())
            return mst;

        std::unordered_map<size_t, size_t> nodeToIdx;
        for (size_t i = 0; i < nodes.size(); ++i)
            nodeToIdx[nodes[i]] = i;

        std::vector<MSTEdge> sortedEdges = edges;
        std::sort(sortedEdges.begin(), sortedEdges.end(), weightComparator);
        DSU dsu(nodes.size());

        for (auto &e : sortedEdges) {
            size_t u = nodeToIdx[e.source], v = nodeToIdx[e.target];

            if (dsu.find(u) != dsu.find(v)) {
                mst.push_back(e);
                dsu.unite(u, v);
            }
        }

        return mst;
    }

// using min heap for optimization vs N^2
std::vector<MSTEdge> buildMSTPrim(const std::vector<size_t>& nodes,
                                  const std::vector<MSTEdge>& edges,
                                  size_t root)
{
    std::vector<MSTEdge> mst;
    if (nodes.empty() || edges.empty()) return mst;

    std::unordered_map<size_t, size_t> nodeToIdx;
    for (size_t i = 0; i < nodes.size(); ++i)
        nodeToIdx[nodes[i]] = i;

    size_t n = nodes.size();
    std::vector<bool> inMST(n, false);

    std::vector<std::vector<MSTEdge>> adj(n);
    for (const auto& e : edges) {
        size_t u = nodeToIdx[e.source], v = nodeToIdx[e.target];
        adj[u].push_back({e.source, e.target, e.weight});
        adj[v].push_back({e.target, e.source, e.weight});
    }

    // Min-heap: (weight, from, to)
    using HeapElem = std::tuple<double, size_t, size_t>;
    std::priority_queue<HeapElem, std::vector<HeapElem>, std::greater<HeapElem>> pq;

    size_t start = nodeToIdx[root];
    inMST[start] = true;
    for (const auto& e : adj[start]) {
        pq.emplace(e.weight, start, nodeToIdx[e.target]);
    }

    while (!pq.empty()) {
        auto [w, u, v] = pq.top();
        pq.pop();
        
        if (inMST[v])
            continue;
        
        inMST[v] = true;
        mst.push_back({nodes[u], nodes[v], w});
        for (const auto& e : adj[v]) {
            size_t to = nodeToIdx[e.target];
            if (!inMST[to]) {
                pq.emplace(e.weight, v, to);
            }
        }
    }
    return mst;
}

} // namespace detail
} // namespace cv