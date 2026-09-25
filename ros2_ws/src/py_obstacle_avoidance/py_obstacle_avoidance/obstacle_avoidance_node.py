from typing import Optional
import math

import rclpy
from geometry_msgs.msg import Twist
from rclpy.node import Node
from sensor_msgs.msg import LaserScan


class ObstacleAvoidance(Node):
    def __init__(self) -> None:
        super().__init__("obstacle_avoidance")

        self.declare_parameter("forward_angle_deg", 30.0)
        self.declare_parameter("safe_distance", 1.0)
        self.declare_parameter("stop_distance", 0.5)
        self.declare_parameter("forward_speed", 0.3)
        self.declare_parameter("slow_speed", 0.15)
        self.declare_parameter("turn_speed", 1.0)

        self.forward_angle_deg: float = self.get_parameter("forward_angle_deg").value
        self.safe_distance: float = self.get_parameter("safe_distance").value
        self.stop_distance: float = self.get_parameter("stop_distance").value
        self.forward_speed: float = self.get_parameter("forward_speed").value
        self.slow_speed: float = self.get_parameter("slow_speed").value
        self.turn_speed: float = self.get_parameter("turn_speed").value

        self.latest_scan: Optional[LaserScan] = None

        self.scan_sub = self.create_subscription(
            LaserScan, "scan", self.scan_callback, 10
        )

        self.cmd_pub = self.create_publisher(Twist, "cmd_vel", 10)

        self.timer = self.create_timer(0.1, self.control_loop)

        self.get_logger().info(
            f"Obstacle avoidance started: "
            f"safe={self.safe_distance} m, stop={self.stop_distance} m"
        )

    def scan_callback(self, msg: LaserScan) -> None:
        self.latest_scan = msg

    def control_loop(self) -> None:
        if self.latest_scan is None:
            return

        scan = self.latest_scan
        n = len(scan.ranges)

        forward_half_angle_rad = math.radians(self.forward_angle_deg)
        angle_increment = scan.angle_increment
        half_samples = int(forward_half_angle_rad / angle_increment)

        center = n // 2

        front_start = max(0, center - half_samples)
        front_end = min(n, center + half_samples)
        front_min = self.min_valid_range(scan.ranges[front_start:front_end])

        left_start = min(n, center + half_samples)
        left_end = min(n, center + 3 * half_samples)
        left_min = self.min_valid_range(scan.ranges[left_start:left_end])

        right_start = max(0, center - 3 * half_samples)
        right_end = max(0, center - half_samples)
        right_min = self.min_valid_range(scan.ranges[right_start:right_end])

        cmd = Twist()

        if front_min < self.stop_distance:
            # stop and turn
            cmd.linear.x = 0.0
            if left_min > right_min:
                cmd.angular.z = self.turn_speed  # turn left
            else:
                cmd.angular.z = -self.turn_speed  # turn right
            self.get_logger().info(
                f"Stop: front={front_min:.2f}, "
                f"left={left_min:.2f}, right={right_min:.2f}",
                throttle_duration_sec=1.0,
            )
        elif front_min < self.safe_distance:
            # 前方有障碍物但还没到危险距离，减速
            cmd.linear.x = self.slow_speed
            cmd.angular.z = 0.0
        else:
            cmd.linear.x = self.forward_speed
            cmd.angular.z = 0.0

        self.cmd_pub.publish(cmd)

    @staticmethod
    def min_valid_range(range: list) -> float:
        """
        从一段 ranges 中取最小值，忽略 inf 和 nan
        """
        valid = [r for r in range if not math.isinf(r) and not math.isnan(r)]
        if not valid:
            return float("inf")
        return min(valid)


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = ObstacleAvoidance()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
