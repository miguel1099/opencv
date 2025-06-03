// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "test_precomp.hpp"
#include <opencv2/3d/detail/mst.hpp>

namespace opencv_test {
namespace {

using namespace cv;

typedef tuple<bool /*isPrim (or Kruskal)*/,
              std::vector<size_t> /*nodes*/,
              std::vector<detail::MSTEdge>/*edges*/,
              std::vector<detail::MSTEdge>/*expectedEdges*/,
              bool /*isConnected*/
             > MSTParamType;
typedef testing::TestWithParam<MSTParamType> MST;
TEST_P(MST, Graphs)
{
    const int isPrim = get<0>(GetParam());
    const std::vector<size_t>& nodes = get<1>(GetParam());
    const std::vector<detail::MSTEdge>& edges = get<2>(GetParam());
    const std::vector<detail::MSTEdge>& expectedEdges = get<3>(GetParam());
    const bool isConnected = get<4>(GetParam());

    std::vector<detail::MSTEdge> mstEdges;
    if (isPrim)
        mstEdges = detail::buildMSTPrim(nodes, edges, 0);
    else
        mstEdges = detail::buildMSTKruskal(nodes, edges);

    // Should have N-1 edges in the MST (if it's connected)
    if (isConnected) {
        EXPECT_EQ(mstEdges.size(), nodes.size() - 1);
    }

    // Check that the MST contains the expected edges
    for (const auto& edge : expectedEdges)
    {
        auto it = std::find_if(mstEdges.begin(), mstEdges.end(),
                               [&edge](const detail::MSTEdge& e) {
                                   return (e.source == edge.source && e.target == edge.target) ||
                                          (e.source == edge.target && e.target == edge.source);
                               });
        EXPECT_NE(it, mstEdges.end()) << "Edge not found in PrimMST: " << edge.source << " -> " << edge.target;
    }
}

// TODO: More test cases
const MSTParamType mst_params[] =
{
    // Prim

    // Small Graph
    MSTParamType(true, {0, 1, 2, 3},
        {
            {0, 1, 1.0}, {0, 2, 2.0}, {1, 2, 1.5}, {1, 3, 2.5}, {2, 3, 1.0}
        },
        {
            {0, 1, 1.0}, {1, 2, 1.5}, {2, 3, 1.0}
        },
        true
    ),
    // Disconnected Graph
    MSTParamType(true, {0, 1, 2},
        {
            {0, 1, 1.0}
        },
        {
            {0, 1, 1.0}
        },
        false
    ),
    // Single Node
    MSTParamType(true, {0},
        {

        },
        {

        },
        true
    ),
    // 2 Nodes, 1 Edge
    MSTParamType(true, {0, 1},
        {
            {0, 1, 42.0}
        },
        {
            {0, 1, 42.0}
        },
        true
    ),
    // Dense graph (clique)
    MSTParamType(true, {0, 1, 2, 3},
        {
            {0, 1, 1.0}, {0, 2, 2.0}, {0, 3, 3.0},
            {1, 2, 1.5}, {1, 3, 2.5}, {2, 3, 1.0}
        },
        {
            {0, 1, 1.0}, {2, 3, 1.0}, {1, 2, 1.5}
        },
        true
    ),

    // Sparse but connected
    MSTParamType(true, {0, 1, 2, 3},
        {
            {0, 1, 1.0}, {1, 2, 2.0}, {2, 3, 3.0}
        },
        {
            {0, 1, 1.0}, {1, 2, 2.0}, {2, 3, 3.0}
        },
        true
    ),

    // Multiple disconnected components
    MSTParamType(true, {0, 1, 2, 3, 4},
        {
            {0, 1, 1.0}, {2, 3, 1.0}
        },
        {
            {0, 1, 1.0}
        },
        false
    ),

    // Kruskal

    // Small Graph
    MSTParamType(false, {0, 1, 2, 3},
        {
            {0, 1, 1.0}, {0, 2, 2.0}, {1, 2, 1.5}, {1, 3, 2.5}, {2, 3, 1.0}
        },
        {
            {0, 1, 1.0}, {1, 2, 1.5}, {2, 3, 1.0}
        },
        true
    ),
    // Disconnected Graph
    MSTParamType(false, {0, 1, 2},
        {
            {0, 1, 1.0}
        },
        {

        },
        false
    ),
    // Single Node
    MSTParamType(false, {0},
        {

        },
        {

        },
        true
    ),
    // 2 Nodes, 1 Edge
    MSTParamType(false, {0, 1},
        {
            {0, 1, 42.0}
        },
        {
            {0, 1, 42.0}
        },
        true
    ),
    // Dense graph (clique)
    MSTParamType(false, {0, 1, 2, 3},
        {
            {0, 1, 1.0}, {0, 2, 2.0}, {0, 3, 3.0},
            {1, 2, 1.5}, {1, 3, 2.5}, {2, 3, 1.0}
        },
        {
            {0, 1, 1.0}, {2, 3, 1.0}, {1, 2, 1.5}
        },
        true
    ),

    // Sparse but connected
    MSTParamType(false, {0, 1, 2, 3},
        {
            {0, 1, 1.0}, {1, 2, 2.0}, {2, 3, 3.0}
        },
        {
            {0, 1, 1.0}, {1, 2, 2.0}, {2, 3, 3.0}
        },
        true
    ),

    // Multiple disconnected components
    MSTParamType(false, {0, 1, 2, 3, 4},
        {
            {0, 1, 1.0}, {2, 3, 1.0}
        },
        {
            {0, 1, 1.0}
        },
        false
    ),
};

inline static std::string MST_name_printer(const testing::TestParamInfo<MST::ParamType>& info)
{ // TODO: better naming!!!
    std::ostringstream os;
    const bool isPrim = get<0>(info.param);
    const auto& nodes = get<1>(info.param);
    const auto& edges = get<2>(info.param);
    const auto& expectedEdges = get<3>(info.param);
    const auto& isConnected = get<4>(info.param);

    os << "MST_" << (isPrim ? "Prim" : "Kruskal") << "_";
    os << "Nodes_" << nodes.size() << "_";
    os << "Edges_" << edges.size() << "_";
    os << "ExpectedEdges_" << expectedEdges.size();
    os << "isConnected_" << isConnected;

    return os.str();
}

INSTANTIATE_TEST_CASE_P(/**/, MST, testing::ValuesIn(mst_params), MST_name_printer);

}} // namespace

