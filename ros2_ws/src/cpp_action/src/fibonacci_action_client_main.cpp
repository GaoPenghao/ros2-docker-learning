#include <memory>

#include "cpp_action/fibonacci_action_client.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  auto node =
      std::make_shared<learning_ros2::cpp_action::FibonacciActionClient>();
  node->send_goal(10, 3);

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
