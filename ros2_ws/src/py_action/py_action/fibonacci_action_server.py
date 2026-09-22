from typing import Optional

import rclpy
from rclpy.action import ActionServer, CancelResponse, GoalResponse
from rclpy.action.server import ServerGoalHandle
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor
from tutorial_interfaces.action import Fibonacci


class FibonacciActionServer(Node):
    def __init__(self) -> None:
        super().__init__("fibonacci_action_server")
        self.action_server = ActionServer(
            self,
            Fibonacci,
            "fibonacci",
            execute_callback=self.execute_callback,
            goal_callback=self.goal_callback,
            cancel_callback=self.cancel_callback,
        )
        self.rate = self.create_rate(1.0)

    def goal_callback(self, goal_request: Fibonacci.Goal) -> GoalResponse:
        self.get_logger().info("Received goal request")
        return GoalResponse.ACCEPT

    def cancel_callback(self, goal_handel: ServerGoalHandle) -> CancelResponse:
        self.get_logger().info("Received cancel request")
        return CancelResponse.ACCEPT

    def execute_callback(self, goal_handle: ServerGoalHandle) -> Fibonacci.Result:
        self.get_logger().info("Executing goal...")

        feedback_msg = Fibonacci.Feedback()
        feedback_msg.partial_sequence = [0, 1]

        for i in range(1, goal_handle.request.order):
            if goal_handle.is_cancel_requested:
                self.get_logger().info("Goal canceled")
                goal_handle.canceled()
                result = Fibonacci.Result()
                result.sequence = feedback_msg.partial_sequence
                return result

            feedback_msg.partial_sequence.append(
                feedback_msg.partial_sequence[i] + feedback_msg.partial_sequence[i - 1]
            )
            self.get_logger().info(f"Feedback: {feedback_msg.partial_sequence}")
            goal_handle.publish_feedback(feedback_msg)
            self.rate.sleep()

        goal_handle.succeed()

        result = Fibonacci.Result()
        result.sequence = feedback_msg.partial_sequence
        return result


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    fibonacci_action_server = FibonacciActionServer()
    executor = MultiThreadedExecutor()
    executor.add_node(fibonacci_action_server)
    try:
        executor.spin()
    finally:
        executor.shutdown()
        fibonacci_action_server.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
