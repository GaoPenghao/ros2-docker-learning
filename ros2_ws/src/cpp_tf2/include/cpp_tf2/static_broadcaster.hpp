#ifndef CPP_TF2__STATIC_BROADCASTER_HPP_
#define CPP_TF2__STATIC_BROADCASTER_HPP_

#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.hpp"

namespace learning_ros2::cpp_tf2 {

class StaticFrameBroadcaster : public rclcpp::Node {
 public:
  explicit StaticFrameBroadcaster(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void publish_static_transform();

 private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_broadcaster_;
};

}  // namespace learning_ros2::cpp_tf2

#endif
