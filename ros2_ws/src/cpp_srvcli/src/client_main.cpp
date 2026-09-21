#include <memory>

#include "cpp_srvcli/client_node.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  if (argc != 3) {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "usage: client X Y");
    rclcpp::shutdown();
    return 1;
  }

  auto node = std::make_shared<learning_ros2::cpp_srvcli::ClientNode>();
  bool success = node->send_request(std::atoll(argv[1]), std::atoll(argv[2]));

  rclcpp::shutdown();

  return success ? 0 : 1;
}
