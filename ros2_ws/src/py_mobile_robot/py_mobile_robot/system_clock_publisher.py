from typing import Optional

import rclpy
from rclpy.node import Node
from rosgraph_msgs.msg import Clock


class SystemClockPublisher(Node):
    def __init__(self) -> None:
        super().__init__("system_clock_publisher")
        self.publisher = self.create_publisher(Clock, "/clock", 10)
        self.timer = self.create_timer(0.01, self.publish_clock)
        self.get_logger().info("System clock publisher started")

    def publish_clock(self) -> None:
        msg = Clock()
        msg.clock = self.get_clock().now().to_msg()
        self.publisher.publish(msg)


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = SystemClockPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
