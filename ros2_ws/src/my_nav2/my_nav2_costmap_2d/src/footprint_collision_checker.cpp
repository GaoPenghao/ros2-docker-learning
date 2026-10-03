#include "my_nav2_costmap_2d/footprint_collision_checker.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

#include "my_nav2_costmap_2d/cost_values.hpp"
#include "my_nav2_costmap_2d/costmap_2d.hpp"
#include "my_nav2_util/line_iterator.hpp"

namespace my_nav2::costmap_2d {

template <typename CostmapT>
FootprintCollisionChecker<CostmapT>::FootprintCollisionChecker()
    : costmap_(nullptr) {}

template <typename CostmapT>
FootprintCollisionChecker<CostmapT>::FootprintCollisionChecker(CostmapT costmap)
    : costmap_(costmap) {}

template <typename CostmapT>
double FootprintCollisionChecker<CostmapT>::footprintCost(
    const Footprint& footprint) {
  // now we really have to lay down the footprint in the costmap_ grid
  unsigned int x0, x1, y0, y1;
  double footprint_cost = 0.0;

  // get the cell coord of the first point
  if (!worldToMap(footprint[0].x, footprint[0].y, x0, y0)) {
    return static_cast<double>(LETHAL_OBSTACLE);
  }

  // cache the start to eliminate a worldToMap call
  unsigned int xstart = x0;
  unsigned int ystart = y0;

  // we need to rasterize each line in the footprint
  for (unsigned int i = 0; i + 1 < footprint.size(); ++i) {
    // get the cell coord of the second point
    if (!worldToMap(footprint[i + 1].x, footprint[i + 1].y, x1, y1)) {
      return static_cast<double>(LETHAL_OBSTACLE);
    }

    footprint_cost = std::max(lineCost(x0, x1, y0, y1), footprint_cost);

    // the second point is next iteration's first point
    x0 = x1;
    y0 = y1;

    // if in collision, no need to continue
    if (footprint_cost == static_cast<double>(LETHAL_OBSTACLE)) {
      return footprint_cost;
    }
  }

  // we also need to connect the first point in the footprint to the last point
  // the last iteration's x1, y1 are the last footprint point's coordinates
  return std::max(lineCost(xstart, x1, ystart, y1), footprint_cost);
}

template <typename CostmapT>
double FootprintCollisionChecker<CostmapT>::footprintCostAtPose(
    double x, double y, double theta, const Footprint& footprint) {
  double cos_th = std::cos(theta);
  double sin_th = std::sin(theta);
  Footprint oriented_footprint;
  oriented_footprint.reserve(footprint.size());
  geometry_msgs::msg::Point new_pt;
  for (const auto& corner_pt : footprint) {
    new_pt.x = x + (corner_pt.x * cos_th - corner_pt.y * sin_th);
    new_pt.y = y + (corner_pt.x * sin_th + corner_pt.y * cos_th);
    oriented_footprint.emplace_back(new_pt);
  }

  return footprintCost(oriented_footprint);
}

template <typename CostmapT>
double FootprintCollisionChecker<CostmapT>::lineCost(int x0, int x1, int y0,
                                                     int y1) const {
  double line_cost = 0.0;
  double point_cost = -1.0;

  for (my_nav2::util::LineIterator line(x0, y0, x1, y1); line.isValid();
       line.advance()) {
    point_cost =
        pointCost(line.getX(), line.getY());  // Score the current point

    // if in collision, no need to continue
    if (point_cost == static_cast<double>(LETHAL_OBSTACLE)) {
      return point_cost;
    }

    if (line_cost < point_cost) {
      line_cost = point_cost;
    }
  }

  return line_cost;
}

template <typename CostmapT>
bool FootprintCollisionChecker<CostmapT>::worldToMap(double wx, double wy,
                                                     unsigned int& mx,
                                                     unsigned int& my) {
  return costmap_->worldToMap(wx, wy, mx, my);
}

template <typename CostmapT>
double FootprintCollisionChecker<CostmapT>::pointCost(int x, int y) const {
  return static_cast<double>(costmap_->getCost(x, y));
}

template <typename CostmapT>
void FootprintCollisionChecker<CostmapT>::setCostmap(CostmapT costmap) {
  costmap_ = costmap;
}

// declare our valid template parameters
template class FootprintCollisionChecker<
    std::shared_ptr<my_nav2::costmap_2d::Costmap2D>>;
template class FootprintCollisionChecker<my_nav2::costmap_2d::Costmap2D*>;

}  // namespace my_nav2::costmap_2d
