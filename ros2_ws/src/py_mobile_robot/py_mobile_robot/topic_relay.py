# 这个 relay 做两件事：
# 1. 把 diff_drive_controller 的 odom 从带前缀的话题转发到 /odom
# 2. 把 /cmd_vel (Twist) 转换成 /diff_drive_controller/cmd_vel (TwistStamped)
# 将来 ros2_control 支持话题名和类型配置后，可以去掉这个 relay

from typing import Optional

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, TwistStamped
from nav_msgs.msg import Odometry


class MobileRobotRelay(Node):

    def __init__(self) -> None:
        super().__init__("mobile_robot_relay")

        # odom: /diff_drive_controller/odom → /odom
        self.odom_pub = self.create_publisher(Odometry, "/odom", 10)
        self.odom_sub = self.create_subscription(
            Odometry, "/diff_drive_controller/odom", self.odom_callback, 10
        )

        # cmd_vel: /cmd_vel (Twist) → /diff_drive_controller/cmd_vel (TwistStamped)
        self.cmd_pub = self.create_publisher(
            TwistStamped, "/diff_drive_controller/cmd_vel", 10
        )
        self.cmd_sub = self.create_subscription(
            Twist, "/cmd_vel", self.cmd_callback, 10
        )

        self.get_logger().info("Mobile robot relay started")

    def odom_callback(self, msg: Odometry) -> None:
        self.odom_pub.publish(msg)

    def cmd_callback(self, msg: Twist) -> None:
        stamped = TwistStamped()
        stamped.header.stamp = self.get_clock().now().to_msg()
        stamped.header.frame_id = "base_link"
        stamped.twist = msg
        self.cmd_pub.publish(stamped)


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = MobileRobotRelay()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
