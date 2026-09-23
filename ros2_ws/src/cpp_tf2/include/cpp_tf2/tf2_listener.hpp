#ifndef CPP_TF2__TF2_LISTENER_HPP_
#define CPP_TF2__TF2_LISTENER_HPP_

#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.hpp"
#include "tf2_ros/transform_listener.hpp"

namespace learning_ros2::cpp_tf2 {

class FrameListener : public rclcpp::Node {
 public:
  explicit FrameListener(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void timer_callback();

 private:
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace learning_ros2::cpp_tf2

#endif
