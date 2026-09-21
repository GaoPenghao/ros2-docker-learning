#ifndef CPP_PUBSUB__SUBSCRIBER_NODE_HPP_
#define CPP_PUBSUB__SUBSCRIBER_NODE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

namespace learning_ros2::cpp_pubsub {

class SubscriberNode : public rclcpp::Node {
 public:
  explicit SubscriberNode(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void topic_callback(const std_msgs::msg::String::SharedPtr msg);

 private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

}  // namespace learning_ros2::cpp_pubsub

#endif  // CPP_PUBSUB__SUBSCRIBER_NODE_HPP_
