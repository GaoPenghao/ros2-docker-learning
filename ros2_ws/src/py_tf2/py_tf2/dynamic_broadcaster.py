from typing import Optional

import math
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
from tf2_ros.transform_broadcaster import TransformBroadcaster


class DynamicFrameBroadcaster(Node):
    def __init__(self) -> None:
        super().__init__("dynamic_frame_broadcaster")
        self.tf_broadcaster = TransformBroadcaster(self)

        self.timer = self.create_timer(0.1, self.timer_callback)

        # 圆周运动参数
        self.radius: float = 1.0
        self.angular_speed: float = 1.0
        self.angle: float = 0.0
        self.dt: float = 0.1

    def timer_callback(self) -> None:
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = "base_link"
        t.child_frame_id = "moving_frame"

        t.transform.translation.x = self.radius * math.cos(self.angle)
        t.transform.translation.y = self.radius * math.sin(self.angle)
        t.transform.translation.z = 0.0

        yaw = self.angle + math.pi / 2.0
        t.transform.rotation.x = 0.0
        t.transform.rotation.y = 0.0
        t.transform.rotation.z = math.sin(yaw / 2.0)
        t.transform.rotation.w = math.cos(yaw / 2.0)

        self.tf_broadcaster.sendTransform(t)

        self.angle += self.angular_speed * self.dt
        if self.angle > 2.0 * math.pi:
            self.angle -= 2.0 * math.pi


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = DynamicFrameBroadcaster()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
