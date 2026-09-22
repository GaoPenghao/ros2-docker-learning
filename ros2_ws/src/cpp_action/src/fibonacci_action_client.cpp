#include "cpp_action/fibonacci_action_client.hpp"

#include <chrono>
#include <sstream>

namespace learning_ros2::cpp_action {

FibonacciActionClient::FibonacciActionClient(const rclcpp::NodeOptions& options)
    : rclcpp::Node("fibonacci_action_client", options),
      cancel_after_(-1),
      feedback_count_(0) {
  client_ = rclcpp_action::create_client<Fibonacci>(this, "fibonacci");
}

void FibonacciActionClient::send_goal(int32_t order, int32_t cancel_after) {
  cancel_after_ = cancel_after;
  feedback_count_ = 0;

  if (!client_->wait_for_action_server(std::chrono::seconds(10))) {
    RCLCPP_ERROR(this->get_logger(),
                 "Action server not available after waiting");
    rclcpp::shutdown();
    return;
  }

  auto goal_msg = Fibonacci::Goal();
  goal_msg.order = order;

  RCLCPP_INFO(this->get_logger(), "Sending goal");

  auto send_goal_options = rclcpp_action::Client<Fibonacci>::SendGoalOptions();
  send_goal_options.goal_response_callback =
      std::bind(&FibonacciActionClient::goal_response_callback, this,
                std::placeholders::_1);
  send_goal_options.feedback_callback =
      std::bind(&FibonacciActionClient::feedback_callback, this,
                std::placeholders::_1, std::placeholders::_2);
  send_goal_options.result_callback = std::bind(
      &FibonacciActionClient::result_callback, this, std::placeholders::_1);

  client_->async_send_goal(goal_msg, send_goal_options);
}

void FibonacciActionClient::goal_response_callback(
    const GoalHandleFibonacci::SharedPtr& goal_handle) {
  if (!goal_handle) {
    RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    rclcpp::shutdown();
    return;
  }

  RCLCPP_INFO(this->get_logger(),
              "Goal accepted by server, waiting for result");
  goal_handle_ = goal_handle;
}

void FibonacciActionClient::feedback_callback(
    GoalHandleFibonacci::SharedPtr,
    const std::shared_ptr<const Fibonacci::Feedback> feedback) {
  feedback_count_++;
  std::stringstream ss;
  ss << "Received feedback #" << feedback_count_ << ": [";
  for (size_t i = 0; i < feedback->partial_sequence.size(); ++i) {
    ss << feedback->partial_sequence[i];
    if (i + 1 < feedback->partial_sequence.size()) {
      ss << ", ";
    }
  }
  ss << "]";
  RCLCPP_INFO(this->get_logger(), "%s", ss.str().c_str());

  if (cancel_after_ > 0 && feedback_count_ >= cancel_after_) {
    RCLCPP_INFO(this->get_logger(), "Requesting cancel...");
    client_->async_cancel_goal(goal_handle_);
  }
}

void FibonacciActionClient::result_callback(
    const GoalHandleFibonacci::WrappedResult& result) {
  switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_INFO(this->get_logger(), "Goal succeeded");
      break;
    case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_INFO(this->get_logger(), "Goal canceled");
      break;
    case rclcpp_action::ResultCode::ABORTED:
      RCLCPP_ERROR(this->get_logger(), "Goal aborted");
      break;
    default:
      RCLCPP_ERROR(this->get_logger(), "Unknown result code");
      break;
  }

  std::stringstream ss;
  ss << "Result: [";
  for (size_t i = 0; i < result.result->sequence.size(); ++i) {
    ss << result.result->sequence[i];
    if (i + 1 < result.result->sequence.size()) {
      ss << ", ";
    }
  }
  ss << "]";
  RCLCPP_INFO(this->get_logger(), "%s", ss.str().c_str());

  rclcpp::shutdown();
}

}  // namespace learning_ros2::cpp_action
