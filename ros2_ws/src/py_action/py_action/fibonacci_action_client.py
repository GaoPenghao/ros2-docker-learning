from typing import Optional

import rclpy
from rclpy.action import ActionClient
from rclpy.action.client import ClientGoalHandle
from rclpy.node import Node
from tutorial_interfaces.action import Fibonacci


class FibonacciActionClient(Node):
    def __init__(self) -> None:
        super().__init__("fibonacci_action_client")
        self.action_client = ActionClient(self, Fibonacci, "fibonacci")
        self.goal_handle: Optional[ClientGoalHandle] = None
        self.feedback_count: int = 0
        self.cancle_after: int = -1

    def send_goal(self, order: int, cancel_after: int = -1) -> None:
        goal_msg = Fibonacci.Goal()
        goal_msg.order = order

        self.cancle_after = cancel_after
        self.feedback_count = 0

        self.action_client.wait_for_server()

        self.send_goal_future = self.action_client.send_goal_async(
            goal_msg, feedback_callback=self.feedback_callback
        )

        self.send_goal_future.add_done_callback(self.goal_response_callback)

    def feedback_callback(self, feedback_msg) -> None:
        feedback: Fibonacci.Feedback = feedback_msg.feedback
        self.feedback_count += 1
        self.get_logger().info(
            f"Received feedback #{self.feedback_count}: {feedback.partial_sequence}"
        )

        if self.feedback_count > 0 and self.feedback_count >= self.cancle_after:
            self.get_logger().info("Requesting cancle...")
            self.goal_handle.cancel_goal_async()

    def goal_response_callback(self, future) -> None:
        goal_handle: ClientGoalHandle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info("Goal rejected :(")
            return

        self.get_logger().info("Goal accepted :)")
        self.goal_handle = goal_handle

        self.get_result_future = goal_handle.get_result_async()
        self.get_result_future.add_done_callback(self.get_result_callback)

    def get_result_callback(self, future) -> None:
        result: Fibonacci.Result = future.result().result
        status = future.result().status
        self.get_logger().info(f"Final status: {status}")
        self.get_logger().info(f"Result: {result.sequence}")
        rclpy.shutdown()


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    action_client = FibonacciActionClient()
    action_client.send_goal(10, cancel_after=3)
    rclpy.spin(action_client)


if __name__ == "__main__":
    main()
