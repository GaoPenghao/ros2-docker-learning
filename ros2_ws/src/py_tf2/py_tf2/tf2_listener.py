from typing import Optional

import math
import rclpy
from rclpy.node import Node
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener
from tf2_ros import LookupException, ConnectivityException, ExtrapolationException
from rclpy.time import Time


class FrameListener(Node):
    def __init__(self) -> None:
        super().__init__("frame_listener")

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        self.timer = self.create_timer(1.0, self.timer_callback)

    def timer_callback(self) -> None:
        try:
            t = self.tf_buffer.lookup_transform("base_link", "moving_frame", Time())

            translation = t.transform.translation
            rotation = t.transform.rotation

            distance = math.sqrt(translation.x**2 + translation.y**2 + translation.z**2)

            self.get_logger().info(
                f"moving_frame at ({translation.x:.3f}, {translation.y:.3f}, {translation.z:.3f}), "
                f"distance from base_link = {distance:.3f} m, "
                f"rotation = ({rotation.x:.3f}, {rotation.y:.3f}, {rotation.z:.3f}, {rotation.w:.3f})"
            )

        except (LookupException, ConnectivityException, ExtrapolationException) as e:
            self.get_logger().warn(f"Could not transform: {e}")


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = FrameListener()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
