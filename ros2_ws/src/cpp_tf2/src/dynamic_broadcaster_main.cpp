#include "cpp_tf2/dynamic_broadcaster.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  auto node =
      std::make_shared<learning_ros2::cpp_tf2::DynamicFrameBroadcaster>();
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
