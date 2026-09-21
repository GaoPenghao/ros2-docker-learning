#include <memory>

#include "cpp_srvcli/service_node.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<learning_ros2::cpp_srvcli::ServiceNode>());
  rclcpp::shutdown();

  return 0;
}
