#include "my_nav2_costmap_2d/footprint.hpp"

#include <cmath>
#include <limits>

#include "my_nav2_costmap_2d/costmap_math.hpp"
#include "my_nav2_util/array_parser.hpp"
#include "rclcpp/rclcpp.hpp"

namespace my_nav2::costmap_2d {

std::pair<double, double> calculateMinAndMaxDistances(
    const std::vector<geometry_msgs::msg::Point>& footprint) {
  double min_dist = std::numeric_limits<double>::max();
  double max_dist = 0.0;

  if (footprint.size() <= 2) {
    return std::pair<double, double>(min_dist, max_dist);
  }

  for (unsigned int i = 0; i < footprint.size() - 1; ++i) {
    // check the distance from the robot center point to the first vertex
    double vertex_dist = distance(0.0, 0.0, footprint[i].x, footprint[i].y);
    double edge_dist = distanceToLine(0.0, 0.0, footprint[i].x, footprint[i].y,
                                      footprint[i + 1].x, footprint[i + 1].y);
    min_dist = std::min(min_dist, std::min(vertex_dist, edge_dist));
    max_dist = std::max(max_dist, std::max(vertex_dist, edge_dist));
  }

  // we also need to do the last vertex and the first vertex
  double vertex_dist =
      distance(0.0, 0.0, footprint.back().x, footprint.back().y);
  double edge_dist =
      distanceToLine(0.0, 0.0, footprint.back().x, footprint.back().y,
                     footprint.front().x, footprint.front().y);
  min_dist = std::min(min_dist, std::min(vertex_dist, edge_dist));
  max_dist = std::max(max_dist, std::max(vertex_dist, edge_dist));

  return std::pair<double, double>(min_dist, max_dist);
}

geometry_msgs::msg::Point toPoint(geometry_msgs::msg::Point32 pt) {
  geometry_msgs::msg::Point point;
  point.x = pt.x;
  point.y = pt.y;
  point.z = pt.z;
  return point;
}

geometry_msgs::msg::Point32 toPoint32(geometry_msgs::msg::Point pt) {
  geometry_msgs::msg::Point32 point32;
  point32.x = pt.x;
  point32.y = pt.y;
  point32.z = pt.z;
  return point32;
}

geometry_msgs::msg::Polygon toPolygon(
    std::vector<geometry_msgs::msg::Point> pts) {
  geometry_msgs::msg::Polygon polygon;
  for (const auto& pt : pts) {
    polygon.points.emplace_back(toPoint32(pt));
  }
  return polygon;
}

std::vector<geometry_msgs::msg::Point> toPointVector(
    geometry_msgs::msg::Polygon::SharedPtr polygon) {
  std::vector<geometry_msgs::msg::Point> pts;
  for (const auto& pt : polygon->points) {
    pts.emplace_back(toPoint(pt));
  }
  return pts;
}

void transformFootprint(
    double x, double y, double theta,
    const std::vector<geometry_msgs::msg::Point>& footprint_spec,
    std::vector<geometry_msgs::msg::Point>& oriented_footprint) {
  // build the oriented footprint at a given location
  oriented_footprint.clear();
  oriented_footprint.reserve(footprint_spec.size());
  double cos_th = cos(theta);
  double sin_th = sin(theta);
  for (const auto& corner_pt : footprint_spec) {
    double new_x = x + (corner_pt.x * cos_th - corner_pt.y * sin_th);
    double new_y = y + (corner_pt.x * sin_th + corner_pt.y * cos_th);
    geometry_msgs::msg::Point new_pt;
    new_pt.x = new_x;
    new_pt.y = new_y;
    oriented_footprint.emplace_back(new_pt);
  }
}

void transformFootprint(
    double x, double y, double theta,
    const std::vector<geometry_msgs::msg::Point>& footprint_spec,
    geometry_msgs::msg::PolygonStamped& oriented_footprint) {
  // build the oriented footprint at a given location
  oriented_footprint.polygon.points.clear();
  oriented_footprint.polygon.points.reserve(footprint_spec.size());
  double cos_th = cos(theta);
  double sin_th = sin(theta);
  for (const auto& corner_pt : footprint_spec) {
    geometry_msgs::msg::Point32 new_pt;
    new_pt.x = x + (corner_pt.x * cos_th - corner_pt.y * sin_th);
    new_pt.y = y + (corner_pt.x * sin_th + corner_pt.y * cos_th);
    oriented_footprint.polygon.points.emplace_back(new_pt);
  }
}

void padFootprint(std::vector<geometry_msgs::msg::Point>& footprint,
                  double padding) {
  // pad footprint in place
  for (auto& pt : footprint) {
    pt.x += sign0(pt.x) * padding;
    pt.y += sign0(pt.y) * padding;
  }
}

std::vector<geometry_msgs::msg::Point> makeFootprintFromRadius(double radius) {
  std::vector<geometry_msgs::msg::Point> points;

  // Loop over 16 angles around a circle making a point each time
  int N = 16;
  const double angle_step = 2 * M_PI / N;
  double angle = 0.0;
  geometry_msgs::msg::Point pt;
  for (int i = 0; i < N; ++i, angle += angle_step) {
    pt.x = cos(angle) * radius;
    pt.y = sin(angle) * radius;

    points.emplace_back(pt);
  }

  return points;
}

bool makeFootprintFromString(
    const std::string& footprint_string,
    std::vector<geometry_msgs::msg::Point>& footprint) {
  std::string error;
  std::vector<std::vector<float>> vvf =
      my_nav2::util::parseVVF(footprint_string, error);

  if (error != "") {
    RCLCPP_ERROR(rclcpp::get_logger("my_nav2_costmap_2d"),
                 "Error parsing footprint parameter: '%s'", error.c_str());
    RCLCPP_ERROR(rclcpp::get_logger("my_nav2_costmap_2d"),
                 "  Footprint string was '%s'.", footprint_string.c_str());
    return false;
  }

  // convert vvf into points.
  if (vvf.size() < 3) {
    RCLCPP_ERROR(
        rclcpp::get_logger("my_nav2_costmap_2d"),
        "You must specify at least three points for the robot footprint,"
        " reverting to previous footprint.");
    return false;
  }
  footprint.clear();
  footprint.reserve(vvf.size());
  for (const auto& vf : vvf) {
    if (vf.size() == 2) {
      geometry_msgs::msg::Point point;
      point.x = vf[0];
      point.y = vf[1];
      point.z = 0;
      footprint.emplace_back(point);
    } else {
      RCLCPP_ERROR(
          rclcpp::get_logger("my_nav2_costmap_2d"),
          "Points in the footprint specification must be pairs of numbers."
          " Found a point with %d numbers.",
          static_cast<int>(vf.size()));
      return false;
    }
  }

  return true;
}

}  // namespace my_nav2::costmap_2d
