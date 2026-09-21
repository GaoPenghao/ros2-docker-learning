#ifndef CPP_SRVCLI__SERVICE_NODE_HPP_
#define CPP_SRVCLI__SERVICE_NODE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/srv/add_two_ints.hpp"

namespace learning_ros2::cpp_srvcli {

class ServiceNode : public rclcpp::Node {
 public:
  using AddTwoInts = tutorial_interfaces::srv::AddTwoInts;

 public:
  explicit ServiceNode(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void handle_add_two_ints(const std::shared_ptr<AddTwoInts::Request> request,
                           std::shared_ptr<AddTwoInts::Response> response);

 private:
  rclcpp::Service<AddTwoInts>::SharedPtr service_;
};

}  // namespace learning_ros2::cpp_srvcli

#endif
