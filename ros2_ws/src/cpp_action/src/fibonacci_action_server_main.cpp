#include "cpp_action/fibonacci_action_server.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  auto node =
      std::make_shared<learning_ros2::cpp_action::FibonacciActionServer>();

  // 使用多线程 executor, 确保取消请求等回调能并发处理
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
