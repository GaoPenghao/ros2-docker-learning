#include "cpp_srvcli/service_node.hpp"

namespace learning_ros2::cpp_srvcli {

ServiceNode::ServiceNode(const rclcpp::NodeOptions& options)
    : rclcpp::Node("minimal_service", options) {
  service_ = this->create_service<AddTwoInts>(
      "add_two_ints", std::bind(&ServiceNode::handle_add_two_ints, this,
                                std::placeholders::_1, std::placeholders::_2));
}

void ServiceNode::handle_add_two_ints(
    const std::shared_ptr<AddTwoInts::Request> request,
    std::shared_ptr<AddTwoInts::Response> response) {
  response->sum = request->a + request->b;
  RCLCPP_INFO(this->get_logger(), "Incoming request\na: %ld b: %ld", request->a,
              request->b);
}

}  // namespace learning_ros2::cpp_srvcli
