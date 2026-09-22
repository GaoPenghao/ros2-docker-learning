#ifndef CPP_ACTION__FIBONACCI_ACTION_CLIENT_HPP_
#define CPP_ACTION__FIBONACCI_ACTION_CLIENT_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "tutorial_interfaces/action/fibonacci.hpp"

namespace learning_ros2::cpp_action {

class FibonacciActionClient : public rclcpp::Node {
 public:
  using Fibonacci = tutorial_interfaces::action::Fibonacci;
  using GoalHandleFibonacci = rclcpp_action::ClientGoalHandle<Fibonacci>;

 public:
  explicit FibonacciActionClient(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

  void send_goal(int32_t order, int32_t cancel_after);

 private:
  void goal_response_callback(
      const GoalHandleFibonacci::SharedPtr& goal_handle);

  void feedback_callback(
      GoalHandleFibonacci::SharedPtr,
      const std::shared_ptr<const Fibonacci::Feedback> feedback);

  void result_callback(const GoalHandleFibonacci::WrappedResult& result);

 private:
  rclcpp_action::Client<Fibonacci>::SharedPtr client_;
  int32_t cancel_after_;
  int32_t feedback_count_;
  GoalHandleFibonacci::SharedPtr goal_handle_;
};

}  // namespace learning_ros2::cpp_action

#endif