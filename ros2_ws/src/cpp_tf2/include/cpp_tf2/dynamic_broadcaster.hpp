#ifndef CPP_TF2__DYNAMIC_BROADCASTER_HPP_
#define CPP_TF2__DYNAMIC_BROADCASTER_HPP_

#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.hpp"

namespace learning_ros2::cpp_tf2 {

class DynamicFrameBroadcaster : public rclcpp::Node {
 public:
  explicit DynamicFrameBroadcaster(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void timer_callback();

 private:
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;

  double radius_;
  double angular_speed_;
  double angle_;
  double dt_;
};

}  // namespace learning_ros2::cpp_tf2

#endif
