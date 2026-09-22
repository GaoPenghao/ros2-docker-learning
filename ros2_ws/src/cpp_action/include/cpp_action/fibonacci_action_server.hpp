#ifndef CPP_ACTION__FIBONACCI_ACTION_SERVER_HPP_
#define CPP_ACTION__FIBONACCI_ACTION_SERVER_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "tutorial_interfaces/action/fibonacci.hpp"

namespace learning_ros2::cpp_action {

class FibonacciActionServer : public rclcpp::Node {
 public:
  using Fibonacci = tutorial_interfaces::action::Fibonacci;
  using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;

 public:
  explicit FibonacciActionServer(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  rclcpp_action::GoalResponse handle_goal(
      const rclcpp_action::GoalUUID& uuid,
      std::shared_ptr<const Fibonacci::Goal> goal);

  rclcpp_action::CancelResponse handle_cancel(
      const std::shared_ptr<GoalHandleFibonacci> goal_handle);

  void handle_accepted(const std::shared_ptr<GoalHandleFibonacci> goal_handle);

  void execute(const std::shared_ptr<GoalHandleFibonacci> goal_handle);

 private:
  rclcpp_action::Server<Fibonacci>::SharedPtr action_server_;
  rclcpp::Rate rate_;
};

}  // namespace learning_ros2::cpp_action

#endif
