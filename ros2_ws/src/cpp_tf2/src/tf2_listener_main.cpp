#include "cpp_tf2/tf2_listener.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<learning_ros2::cpp_tf2::FrameListener>();
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
