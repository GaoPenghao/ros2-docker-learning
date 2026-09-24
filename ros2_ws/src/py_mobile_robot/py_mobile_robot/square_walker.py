from typing import Optional

import rclpy
from geometry_msgs.msg import Twist
from rclpy.node import Node


class SquareWalker(Node):
    def __init__(self) -> None:
        super().__init__("square_walker")

        self.publisher = self.create_publisher(Twist, "cmd_vel", 10)

        self.declare_parameter("straight_time", 2.0)
        self.declare_parameter("turn_time", 1.57)
        self.declare_parameter("linear_speed", 0.2)
        self.declare_parameter("angular_speed", 1.0)

        self.straight_time = self.get_parameter("straight_time").value
        self.turn_time = self.get_parameter("turn_time").value
        self.linear_speed = self.get_parameter("linear_speed").value
        self.angular_speed = self.get_parameter("angular_speed").value

        self.step: int = 0  # 当前处于正方形第几段
        self.phase: str = "straight"  # straight or turn

        self.timer = self.create_timer(self.straight_time, self.step_callback)

        self.cmd_timer = self.create_timer(0.05, self.publish_cmd)

        self.get_logger().info("Square walker started")

    def step_callback(self) -> None:
        if self.phase == "straight":
            self.phase = "turn"
            self.timer.cancel()
            self.timer = self.create_timer(self.turn_time, self.step_callback)
            self.get_logger().info(f"Step {self.step + 1}: turning")
        else:
            self.step += 1
            if self.step >= 4:
                self.get_logger().info("Square completed! Stopping.")
                self.stop()
                return
            self.phase = "straight"
            self.timer.cancel()
            self.timer = self.create_timer(self.straight_time, self.step_callback)
            self.get_logger().info(f"Step {self.step + 1}: going straight")

    def publish_cmd(self) -> None:
        msg = Twist()
        if self.phase == "straight":
            msg.linear.x = self.linear_speed
        elif self.phase == "turn":
            msg.angular.z = self.angular_speed
        self.publisher.publish(msg)

    def stop(self) -> None:
        self.cmd_timer.cancel()
        self.publisher.publish(Twist())


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = SquareWalker()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
