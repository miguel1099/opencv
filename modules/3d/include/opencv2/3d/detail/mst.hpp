// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_3D_DETAIL_MST_HPP
#define OPENCV_3D_DETAIL_MST_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <queue>

namespace cv
{
namespace detail
{

struct MSTEdge
{
    size_t source, target;
    double weight;
};

std::vector<MSTEdge> buildMSTPrim(
    const std::vector<size_t>& nodes, // Should be 0-based
    const std::vector<MSTEdge>& edges,
    size_t root = 0
);

std::vector<MSTEdge> buildMSTKruskal(
    const std::vector<size_t>& nodes,
    const std::vector<MSTEdge>& edges
);

} // namespace detail
} // namespace cv

#endif // include guard