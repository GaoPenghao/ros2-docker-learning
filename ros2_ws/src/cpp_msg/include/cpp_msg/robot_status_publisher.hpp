#ifndef CPP_MSG__ROBOT_STATUS_PUBLISHER_HPP_
#define CPP_MSG__ROBOT_STATUS_PUBLISHER_HPP_

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/msg/robot_status.hpp"

namespace learning_ros2::cpp_msg {

class RobotStatusPublisher : public rclcpp::Node {
 public:
  using RobotStatus = tutorial_interfaces::msg::RobotStatus;

 public:
  explicit RobotStatusPublisher(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void timer_callback();

 private:
  rclcpp::Publisher<RobotStatus>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::string robot_name_;
  double battery_level_;
  double initial_battery_;
  double publish_period_;
};

}  // namespace learning_ros2::cpp_msg

#endif