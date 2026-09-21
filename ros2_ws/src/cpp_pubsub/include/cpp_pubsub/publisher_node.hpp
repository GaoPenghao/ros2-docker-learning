#ifndef CPP_PUBSUB__PUBLISHER_NODE_HPP_
#define CPP_PUBSUB__PUBLISHER_NODE_HPP_

#include <chrono>
#include <cstddef>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

namespace learning_ros2::cpp_pubsub {

class PublisherNode : public rclcpp::Node {
 public:
  explicit PublisherNode(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void timer_callback();

 private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  std::size_t count_ = 0;
};

}  // namespace learning_ros2::cpp_pubsub

#endif  // CPP_PUBSUB__PUBLISHER_NODE_HPP_
