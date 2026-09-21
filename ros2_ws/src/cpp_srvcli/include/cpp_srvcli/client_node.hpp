#ifndef CPP_SRVCLI__CLIENT_NODE_HPP_
#define CPP_SRVCLI__CLIENT_NODE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/srv/add_two_ints.hpp"

namespace learning_ros2::cpp_srvcli {

class ClientNode : public rclcpp::Node {
 public:
  using AddTwoInts = tutorial_interfaces::srv::AddTwoInts;

 public:
  explicit ClientNode(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

  bool send_request(std::int64_t a, std::int64_t b);

 private:
  rclcpp::Client<AddTwoInts>::SharedPtr client_;
};

}  // namespace learning_ros2::cpp_srvcli

#endif
