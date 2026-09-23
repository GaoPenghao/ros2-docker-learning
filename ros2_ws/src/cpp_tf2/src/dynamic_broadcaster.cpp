#include "cpp_tf2/dynamic_broadcaster.hpp"

#include <chrono>
#include <cmath>

namespace learning_ros2::cpp_tf2 {

DynamicFrameBroadcaster::DynamicFrameBroadcaster(
    const rclcpp::NodeOptions& options)
    : rclcpp::Node("dynamic_frame_broadcaster", options),
      radius_(1.0),
      angular_speed_(1.0),
      angle_(0.0),
      dt_(0.1) {
  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

  timer_ = this->create_wall_timer(
      std::chrono::duration<double>(dt_),
      std::bind(&DynamicFrameBroadcaster::timer_callback, this));
}

void DynamicFrameBroadcaster::timer_callback() {
  geometry_msgs::msg::TransformStamped t;

  t.header.stamp = this->get_clock()->now();
  t.header.frame_id = "base_link";
  t.child_frame_id = "moving_frame";

  t.transform.translation.x = radius_ * std::cos(angle_);
  t.transform.translation.y = radius_ * std::sin(angle_);
  t.transform.translation.z = 0.0;

  double yaw = angle_ + M_PI / 2.0;
  t.transform.rotation.x = 0.0;
  t.transform.rotation.y = 0.0;
  t.transform.rotation.z = std::sin(yaw / 2.0);
  t.transform.rotation.w = std::cos(yaw / 2.0);

  tf_broadcaster_->sendTransform(t);

  angle_ += angular_speed_ * dt_;
  if (angle_ > 2.0 * M_PI) {
    angle_ -= 2.0 * M_PI;
  }
}

}  // namespace learning_ros2::cpp_tf2
