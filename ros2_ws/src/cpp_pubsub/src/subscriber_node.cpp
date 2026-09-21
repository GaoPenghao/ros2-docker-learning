#include "cpp_pubsub/subscriber_node.hpp"

namespace learning_ros2::cpp_pubsub {

SubscriberNode::SubscriberNode(const rclcpp::NodeOptions& options)
    : rclcpp::Node("minimal_subscriber", options) {
  subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10,
      std::bind(&SubscriberNode::topic_callback, this, std::placeholders::_1));
}

void SubscriberNode::topic_callback(
    const std_msgs::msg::String::SharedPtr msg) {
  RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
}

}  // namespace learning_ros2::cpp_pubsub
