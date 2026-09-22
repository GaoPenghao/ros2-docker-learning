from typing import Optional

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster


class StaticFrameBroadcaster(Node):
    def __init__(self) -> None:
        super().__init__("static_frame_broadcaster")
        self.tf_broadcaster = StaticTransformBroadcaster(self)
        self.publish_static_transform()

    def publish_static_transform(self) -> None:
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = "base_link"
        t.child_frame_id = "laser_frame"

        # 激光雷达在底盘前方 0.2m, 左方 0m, 上方 0.15m
        t.transform.translation.x = 0.2
        t.transform.translation.y = 0.0
        t.transform.translation.z = 0.15

        # 无旋转，用单位四元数
        t.transform.rotation.x = 0.0
        t.transform.rotation.y = 0.0
        t.transform.rotation.z = 0.0
        t.transform.rotation.w = 1.0

        self.tf_broadcaster.sendTransform(t)
        self.get_logger().info("Published static transform: base_link -> laser_frame")


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = StaticFrameBroadcaster()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
