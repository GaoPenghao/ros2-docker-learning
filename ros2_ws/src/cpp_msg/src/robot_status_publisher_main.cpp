#include "cpp_msg/robot_status_publisher.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<learning_ros2::cpp_msg::RobotStatusPublisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
