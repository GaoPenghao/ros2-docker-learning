from typing import Optional

import rclpy
from rclpy.node import Node
from tutorial_interfaces.msg import RobotStatus


class RobotStatusSubscriber(Node):
    def __init__(self) -> None:
        super().__init__("robot_status_subscriber")
        self.subscription = self.create_subscription(
            RobotStatus, "robot_status", self.listener_callback, 10
        )

    def listener_callback(self, msg: RobotStatus) -> None:
        self.get_logger().info(
            f"Received from {msg.robot_name}: "
            f"battery={msg.battery_level:.1f}, "
            f"joints={msg.joint_positions}"
        )


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = RobotStatusSubscriber()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
