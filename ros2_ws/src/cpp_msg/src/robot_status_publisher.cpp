#include "cpp_msg/robot_status_publisher.hpp"

#include <chrono>

namespace learning_ros2::cpp_msg {

RobotStatusPublisher::RobotStatusPublisher(const rclcpp::NodeOptions& options)
    : rclcpp::Node("robot_status_publisher", options),
      robot_name_("learning_bot"),
      battery_level_(100.0),
      initial_battery_(100.0),
      publish_period_(1.0) {
  this->declare_parameter("robot_name", robot_name_);
  this->declare_parameter("initial_battery", initial_battery_);
  this->declare_parameter("publish_period", publish_period_);

  robot_name_ = this->get_parameter("robot_name").as_string();
  initial_battery_ = this->get_parameter("initial_battery").as_double();
  battery_level_ = initial_battery_;
  publish_period_ = this->get_parameter("publish_period").as_double();

  publisher_ = this->create_publisher<RobotStatus>("robot_status", 10);

  auto period = std::chrono::duration<double>(publish_period_);
  timer_ = this->create_wall_timer(
      std::chrono::duration_cast<std::chrono::milliseconds>(period),
      std::bind(&RobotStatusPublisher::timer_callback, this));

  RCLCPP_INFO(
      this->get_logger(),
      "Configured: robot_name=%s, initial_battery=%.1f, publish_period=%.1f",
      robot_name_.c_str(), initial_battery_, publish_period_);
}

void RobotStatusPublisher::timer_callback() {
  RobotStatus msg;
  msg.robot_name = robot_name_;
  msg.battery_level = battery_level_;
  msg.is_active = true;
  msg.joint_positions = {0.0, 0.5, -0.5, 1.0, -1.0};

  publisher_->publish(msg);

  RCLCPP_INFO(this->get_logger(), "Publishing: robot_name=%s, battery=%.1f",
              msg.robot_name.c_str(), msg.battery_level);

  battery_level_ -= 1.0;
  if (battery_level_ < 0.0) {
    battery_level_ = initial_battery_;
  }
}

}  // namespace learning_ros2::cpp_msg
