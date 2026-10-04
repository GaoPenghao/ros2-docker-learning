#ifndef MY_NAV2_COSTMAP_2D__COSTMAP_2D_PUBLISHER_HPP_
#define MY_NAV2_COSTMAP_2D__COSTMAP_2D_PUBLISHER_HPP_

#include <algorithm>
#include <memory>
#include <string>

#include "map_msgs/msg/occupancy_grid_update.hpp"
#include "my_nav2_costmap_2d/costmap_2d.hpp"
#include "my_nav2_msgs/msg/costmap.hpp"
#include "my_nav2_msgs/msg/costmap_update.hpp"
#include "my_nav2_msgs/srv/get_costmap.hpp"
#include "my_nav2_util/lifecycle_node.hpp"
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

namespace my_nav2::costmap_2d {

/**
 * @class Costmap2DPublisher
 * @brief A tool to periodically publish visualization data from a Costmap2D
 */
class Costmap2DPublisher {
 public:
  /**
   * @brief Constructor for the Costmap2DPublisher
   */
  Costmap2DPublisher(const my_nav2::util::LifecycleNode::WeakPtr& parent,
                     Costmap2D* costmap, std::string global_frame,
                     std::string topic_name,
                     bool always_send_full_costmap = false,
                     double map_vis_z = 0.0);

  /**
   * @brief Destructor
   */
  ~Costmap2DPublisher();

  /**
   * @brief Configure node
   */
  void on_configure() {}

  /**
   * @brief Activate node
   */
  void on_activate() {
    costmap_pub_->on_activate();
    costmap_update_pub_->on_activate();
    costmap_raw_pub_->on_activate();
    costmap_raw_update_pub_->on_activate();
  }

  /**
   * @brief deactivate node
   */
  void on_deactivate() {
    costmap_pub_->on_deactivate();
    costmap_update_pub_->on_deactivate();
    costmap_raw_pub_->on_deactivate();
    costmap_raw_update_pub_->on_deactivate();
  }

  /**
   * @brief Cleanup node
   */
  void on_cleanup() {}

  /**
   * @brief Include the given bounds in the changed-rectangle.
   */
  void updateBounds(unsigned int x0, unsigned int xn, unsigned int y0,
                    unsigned int yn) {
    x0_ = std::min(x0, x0_);
    xn_ = std::max(xn, xn_);
    y0_ = std::min(y0, y0_);
    yn_ = std::max(yn, yn_);
  }

  /**
   * @brief Publishes the visualization data over ROS
   * @note Only publishes when the associated layer is enabled
   */
  void publishCostmap();

 private:
  /**
   * @brief Prepare grid_ message for publication.
   */
  void prepareGrid();

  void prepareCostmap();

  /**
   * @brief Prepare OccupancyGridUpdate msg for publication.
   */
  std::unique_ptr<map_msgs::msg::OccupancyGridUpdate> createGridUpdateMsg();

  /**
   * @brief Prepare CostmapUpdate msg for publication.
   */
  std::unique_ptr<my_nav2_msgs::msg::CostmapUpdate> createCostmapUpdateMsg();

  void updateGridParams();

  /**
   * @brief GetCostmap callback service
   */
  void costmap_service_callback(
      const std::shared_ptr<rmw_request_id_t> request_header,
      const std::shared_ptr<my_nav2_msgs::srv::GetCostmap::Request> request,
      const std::shared_ptr<my_nav2_msgs::srv::GetCostmap::Response> response);

  rclcpp::Clock::SharedPtr clock_;
  rclcpp::Logger logger_{rclcpp::get_logger("my_nav2_costmap_2d")};

  Costmap2D* costmap_;
  std::string global_frame_;
  std::string topic_name_;
  unsigned int x0_{0};
  unsigned int xn_{0};
  unsigned int y0_{0};
  unsigned int yn_{0};
  double saved_origin_x_{0.0};
  double saved_origin_y_{0.0};
  bool always_send_full_costmap_{false};
  bool costmap_published_once_{false};
  double map_vis_z_{0.0};

  // Publisher for translated costmap values as msg::OccupancyGrid used in
  // visualization
  rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::OccupancyGrid>::SharedPtr
      costmap_pub_;
  rclcpp_lifecycle::LifecyclePublisher<
      map_msgs::msg::OccupancyGridUpdate>::SharedPtr costmap_update_pub_;

  // Publisher for raw costmap values as msg::Costmap from layered costmap
  rclcpp_lifecycle::LifecyclePublisher<my_nav2_msgs::msg::Costmap>::SharedPtr
      costmap_raw_pub_;
  rclcpp_lifecycle::LifecyclePublisher<
      my_nav2_msgs::msg::CostmapUpdate>::SharedPtr costmap_raw_update_pub_;

  // Service for getting the costmaps
  rclcpp::Service<my_nav2_msgs::srv::GetCostmap>::SharedPtr costmap_service_;

  float grid_resolution_{0.0};
  unsigned int grid_width_{0};
  unsigned int grid_height_{0};
  std::unique_ptr<nav_msgs::msg::OccupancyGrid> grid_;
  std::unique_ptr<my_nav2_msgs::msg::Costmap> costmap_raw_;
  // Translate from 0-255 values in costmap to -1 to 100 values in message.
  static char* cost_translation_table_;
};

}  // namespace my_nav2::costmap_2d

#endif  // MY_NAV2_COSTMAP_2D__COSTMAP_2D_PUBLISHER_HPP_
