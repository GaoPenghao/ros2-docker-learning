from typing import Optional

import rclpy
from rclpy.node import Node
from rclpy.time import Time
from geometry_msgs.msg import Twist
from sensor_msgs.msg import JointState
from nav_msgs.msg import Odometry
from tf2_ros import TransformBroadcaster


class DiffDriveController(Node):
    def __init__(self) -> None:
        super().__init__("diff_drive_controller")

        # robot param, value keep same with URDF
        self.declare_parameter("wheel_radius", 0.05)
        self.declare_parameter("wheel_separation", 0.34)

        self.wheel_radius: float = self.get_parameter("wheel_radius").value
        self.wheel_separation: float = self.get_parameter("wheel_separation").value

        # robot states
        self.x: float = 0.0
        self.y: float = 0.0
        self.theta: float = 0.0
        self.left_wheel_angle: float = 0.0
        self.right_wheel_angle: float = 0.0

        # current cmd vel
        self.v: float = 0.0
        self.omega: float = 0.0

        # time
        self.dt: float = 0.05
        self.last_time = self.get_clock().now()

        # subscribe /cmd_vel
        self.cmd_vel_sub = self.create_subscription(
            Twist, "cmd_vel", self.cmd_vel_callback, 10
        )

        # publish /joint_states
        self.joint_pub = self.create_publisher(JointState, "joint_states", 10)

        # publish /odom
        self.odom_pub = self.create_publisher(Odometry, "odom", 10)

        # TF broadcaste: odom -> base_link
        self.tf_broadcaster = TransformBroadcaster(self)

        # timer: update and publish pose
        self.timer = self.create_timer(self.dt, self.update_callback)

        self.get_logger().info(
            f"Diff drive controller started: "
            f"wheel_radius={self.wheel_radius}, "
            f"wheel_separation={self.wheel_separation}"
        )

    def cmd_vel_callback(self, msg: Twist) -> None:
        pass

    def update_callback(self) -> None:
        pass

    def publish_joint_states(self, current_time: Time) -> None:
        pass

    def publish_odom(self, current_time: Time) -> None:
        pass
