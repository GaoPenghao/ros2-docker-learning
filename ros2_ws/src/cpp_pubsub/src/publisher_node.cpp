#include "cpp_pubsub/publisher_node.hpp"

namespace learning_ros2::cpp_pubsub {

PublisherNode::PublisherNode(const rclcpp::NodeOptions& options)
    : rclcpp::Node("minimal_publisher", options) {
  publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
  timer_ =
      this->create_wall_timer(std::chrono::milliseconds(500),
                              std::bind(&PublisherNode::timer_callback, this));
}

void PublisherNode::timer_callback() {
  auto message = std_msgs::msg::String();
  message.data = "Hello, world: " + std::to_string(count_++);
  RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
  publisher_->publish(message);
}

}  // namespace learning_ros2::cpp_pubsub
