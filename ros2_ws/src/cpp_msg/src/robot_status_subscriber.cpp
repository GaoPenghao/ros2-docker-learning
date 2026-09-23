#include "cpp_msg/robot_status_subscriber.hpp"

#include <sstream>

namespace learning_ros2::cpp_msg {

RobotStatusSubscriber::RobotStatusSubscriber(const rclcpp::NodeOptions& options)
    : rclcpp::Node("robot_status_subscriber", options) {
  subscription_ = this->create_subscription<RobotStatus>(
      "robot_status", 10,
      std::bind(&RobotStatusSubscriber::topic_callback, this,
                std::placeholders::_1));
}

void RobotStatusSubscriber::topic_callback(
    const RobotStatus::SharedPtr msg) const {
  std::stringstream joints_ss;
  joints_ss << "[";
  for (size_t i = 0; i < msg->joint_positions.size(); ++i) {
    joints_ss << msg->joint_positions[i];
    if (i + 1 < msg->joint_positions.size()) {
      joints_ss << ", ";
    }
  }
  joints_ss << "]";

  RCLCPP_INFO(this->get_logger(),
              "Received from %s: battery=%.1f, active=%s, joints=%s",
              msg->robot_name.c_str(), msg->battery_level,
              msg->is_active ? "true" : "false", joints_ss.str().c_str());
}

}  // namespace learning_ros2::cpp_msg
