#include "cpp_tf2/tf2_listener.hpp"

#include <chrono>

namespace learning_ros2::cpp_tf2 {

FrameListener::FrameListener(const rclcpp::NodeOptions& options)
    : rclcpp::Node("frame_listener", options) {
  tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

  timer_ = this->create_wall_timer(
      std::chrono::seconds(1), std::bind(&FrameListener::timer_callback, this));
}

void FrameListener::timer_callback() {
  geometry_msgs::msg::TransformStamped t;

  try {
    t = tf_buffer_->lookupTransform("base_link", "moving_frame",
                                    tf2::TimePointZero);
  } catch (const tf2::TransformException& ex) {
    RCLCPP_WARN(this->get_logger(), "Could not transform: %s", ex.what());
    return;
  }

  const auto& translation = t.transform.translation;
  const auto& rotation = t.transform.rotation;

  double distance =
      std::sqrt(translation.x * translation.x + translation.y * translation.y +
                translation.z * translation.z);

  RCLCPP_INFO(this->get_logger(),
              "moving_frame at (%.3f, %.3f, %.3f), "
              "distance from base_link = %.3f m, "
              "rotation = (%.3f, %.3f, %.3f, %.3f)",
              translation.x, translation.y, translation.z, distance, rotation.x,
              rotation.y, rotation.z, rotation.w);
}

}  // namespace learning_ros2::cpp_tf2
