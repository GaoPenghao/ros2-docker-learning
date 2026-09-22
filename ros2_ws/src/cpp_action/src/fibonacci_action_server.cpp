#include "cpp_action/fibonacci_action_server.hpp"

#include <thread>

namespace learning_ros2::cpp_action {

FibonacciActionServer::FibonacciActionServer(const rclcpp::NodeOptions& options)
    : rclcpp::Node("fibonacci_action_server", options), rate_(1.0) {
  action_server_ = rclcpp_action::create_server<Fibonacci>(
      this, "fibonacci",
      std::bind(&FibonacciActionServer::handle_goal, this,
                std::placeholders::_1, std::placeholders::_2),
      std::bind(&FibonacciActionServer::handle_cancel, this,
                std::placeholders::_1),
      std::bind(&FibonacciActionServer::handle_accepted, this,
                std::placeholders::_1));
}

rclcpp_action::GoalResponse FibonacciActionServer::handle_goal(
    const rclcpp_action::GoalUUID& uuid,
    std::shared_ptr<const Fibonacci::Goal> goal) {
  RCLCPP_INFO(this->get_logger(), "Received goal request");
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse FibonacciActionServer::handle_cancel(
    const std::shared_ptr<GoalHandleFibonacci> goal_handle) {
  RCLCPP_INFO(this->get_logger(), "Received cancel request");
  return rclcpp_action::CancelResponse::ACCEPT;
}

void FibonacciActionServer::handle_accepted(
    const std::shared_ptr<GoalHandleFibonacci> goal_handle) {
  // 必须在新线程中执行，否则会阻塞 executor, 取消请求无法被处理
  std::thread{
      std::bind(&FibonacciActionServer::execute, this, std::placeholders::_1),
      goal_handle}
      .detach();
}

void FibonacciActionServer::execute(
    const std::shared_ptr<GoalHandleFibonacci> goal_handle) {
  RCLCPP_INFO(this->get_logger(), "Executing goal...");

  auto feedback = std::make_shared<Fibonacci::Feedback>();
  auto& sequence = feedback->partial_sequence;
  sequence.emplace_back(0);
  sequence.emplace_back(1);

  auto result = std::make_shared<Fibonacci::Result>();

  for (int32_t i = 1; i < goal_handle->get_goal()->order; ++i) {
    if (goal_handle->is_canceling()) {
      RCLCPP_INFO(this->get_logger(), "Goal canceled");
      result->sequence = sequence;
      goal_handle->canceled(result);
      return;
    }

    sequence.emplace_back(sequence[i] + sequence[i - 1]);
    goal_handle->publish_feedback(feedback);
    RCLCPP_INFO(this->get_logger(), "Publish feedback");

    rate_.sleep();
  }

  result->sequence = sequence;
  goal_handle->succeed(result);
  RCLCPP_INFO(this->get_logger(), "Goal succeeded");
}

}  // namespace learning_ros2::cpp_action
