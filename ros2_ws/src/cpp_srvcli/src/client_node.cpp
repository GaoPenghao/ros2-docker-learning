#include "cpp_srvcli/client_node.hpp"

namespace learning_ros2::cpp_srvcli {

ClientNode::ClientNode(const rclcpp::NodeOptions& options)
    : rclcpp::Node("minimal_client_async", options) {
  client_ = this->create_client<AddTwoInts>("add_two_ints");
}

bool ClientNode::send_request(std::int64_t a, std::int64_t b) {
  while (!client_->wait_for_service(std::chrono::seconds(1))) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(),
                   "Interrupted while waiting for the service. Exiting.");
      return false;
    }
    RCLCPP_INFO(this->get_logger(), "Service not available, waiting again...");
  }

  auto request = std::make_shared<AddTwoInts::Request>();
  request->a = a;
  request->b = b;

  auto future = client_->async_send_request(request);
  if (rclcpp::spin_until_future_complete(this->get_node_base_interface(),
                                         future) !=
      rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_ERROR(this->get_logger(), "Failed to call service add_two_ints");
    return false;
  }

  auto response = future.get();
  RCLCPP_INFO(this->get_logger(), "Result of add_two_ints: for %ld + %ld = %ld",
              a, b, response->sum);

  return true;
}

}  // namespace learning_ros2::cpp_srvcli
