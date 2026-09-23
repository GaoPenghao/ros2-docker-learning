#ifndef CPP_MSG__ROBOT_STATUS_SUBSCRIBER_HPP_
#define CPP_MSG__ROBOT_STATUS_SUBSCRIBER_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/msg/robot_status.hpp"

namespace learning_ros2::cpp_msg {

class RobotStatusSubscriber : public rclcpp::Node {
 public:
  using RobotStatus = tutorial_interfaces::msg::RobotStatus;

 public:
  explicit RobotStatusSubscriber(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void topic_callback(const RobotStatus::SharedPtr msg) const;

 private:
  rclcpp::Subscription<RobotStatus>::SharedPtr subscription_;
};

}  // namespace learning_ros2::cpp_msg

#endif
