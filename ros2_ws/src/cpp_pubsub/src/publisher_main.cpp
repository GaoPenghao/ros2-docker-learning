#include <memory>

#include "cpp_pubsub/publisher_node.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<learning_ros2::cpp_pubsub::PublisherNode>());
  rclcpp::shutdown();

  return 0;
}
