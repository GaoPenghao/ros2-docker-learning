#ifndef MY_NAV2_COSTMAP_2D__CLEAR_COSTMAP_SERVICE_HPP_
#define MY_NAV2_COSTMAP_2D__CLEAR_COSTMAP_SERVICE_HPP_

#include <memory>

#include "geometry_msgs/msg/pose_stamped.hpp"
#include "my_nav2_costmap_2d/costmap_layer.hpp"
#include "my_nav2_msgs/srv/clear_costmap_around_pose.hpp"
#include "my_nav2_msgs/srv/clear_costmap_around_robot.hpp"
#include "my_nav2_msgs/srv/clear_costmap_except_region.hpp"
#include "my_nav2_msgs/srv/clear_entire_costmap.hpp"
#include "my_nav2_util/lifecycle_node.hpp"
#include "rclcpp/rclcpp.hpp"

namespace my_nav2::costmap_2d {

class Costmap2DROS;

/**
 * @class ClearCostmapService
 * @brief Exposes services to clear costmap objects in inclusive/exclusive
 * regions or completely
 */
class ClearCostmapService {
 public:
  /**
   * @brief A constructor
   */
  ClearCostmapService(const my_nav2::util::LifecycleNode::WeakPtr& parent,
                      Costmap2DROS& costmap);

  /**
   * @brief A constructor
   */
  ClearCostmapService() = delete;

  /**
   * @brief A destructor
   */
  ~ClearCostmapService();

  /**
   * @brief Clears the region outside of a user-specified area reverting to the
   * static map
   */
  void clearRegion(double reset_distance, bool invert);

  /**
   * @brief Clears the region around a specific pose
   */
  void clearAroundPose(const geometry_msgs::msg::PoseStamped& pose,
                       double reset_distance);

  /**
   * @brief Clears all layers
   */
  void clearEntirely();

 private:
  /**
   * @brief Callback to clear costmap except in a given region
   */
  void clearExceptRegionCallback(
      const std::shared_ptr<rmw_request_id_t> request_header,
      const std::shared_ptr<
          my_nav2_msgs::srv::ClearCostmapExceptRegion::Request>
          request,
      const std::shared_ptr<
          my_nav2_msgs::srv::ClearCostmapExceptRegion::Response>
          response);

  /**
   * @brief Callback to clear costmap in a given region
   */
  void clearAroundRobotCallback(
      const std::shared_ptr<rmw_request_id_t> request_header,
      const std::shared_ptr<my_nav2_msgs::srv::ClearCostmapAroundRobot::Request>
          request,
      const std::shared_ptr<
          my_nav2_msgs::srv::ClearCostmapAroundRobot::Response>
          response);

  /**
   * @brief Callback to clear costmap around a given pose
   */
  void clearAroundPoseCallback(
      const std::shared_ptr<rmw_request_id_t> request_header,
      const std::shared_ptr<my_nav2_msgs::srv::ClearCostmapAroundPose::Request>
          request,
      const std::shared_ptr<my_nav2_msgs::srv::ClearCostmapAroundPose::Response>
          response);

  /**
   * @brief Callback to clear costmap
   */
  void clearEntireCallback(
      const std::shared_ptr<rmw_request_id_t> request_header,
      const std::shared_ptr<my_nav2_msgs::srv::ClearEntireCostmap::Request>
          request,
      const std::shared_ptr<my_nav2_msgs::srv::ClearEntireCostmap::Response>
          response);

  /**
   * @brief Function used to clear a given costmap layer
   */
  void clearLayerRegion(std::shared_ptr<CostmapLayer>& costmap, double pose_x,
                        double pose_y, double reset_distance, bool invert);

  /**
   * @brief Get the robot's position in the costmap using the master costmap
   */
  bool getPosition(double& x, double& y) const;

  // The Logger object for logging
  rclcpp::Logger logger_{rclcpp::get_logger("my_nav2_costmap_2d")};

  // The costmap to clear
  Costmap2DROS& costmap_;

  // Clearing parameters
  unsigned char reset_value_;

  // Server for clearing the costmap
  rclcpp::Service<my_nav2_msgs::srv::ClearCostmapExceptRegion>::SharedPtr
      clear_except_service_;
  rclcpp::Service<my_nav2_msgs::srv::ClearCostmapAroundRobot>::SharedPtr
      clear_around_service_;
  rclcpp::Service<my_nav2_msgs::srv::ClearCostmapAroundPose>::SharedPtr
      clear_around_pose_service_;
  rclcpp::Service<my_nav2_msgs::srv::ClearEntireCostmap>::SharedPtr
      clear_entire_service_;
};

}  // namespace my_nav2::costmap_2d

#endif  // MY_NAV2_COSTMAP_2D__CLEAR_COSTMAP_SERVICE_HPP_
