from typing import Optional
import math

import rclpy
from rclpy.node import Node
from rclpy.time import Time
from geometry_msgs.msg import Twist, Quaternion, TransformStamped
from sensor_msgs.msg import JointState
from nav_msgs.msg import Odometry
from tf2_ros import TransformBroadcaster


class DiffDriveController(Node):
    def __init__(self) -> None:
        super().__init__("diff_drive_controller")

        # robot param, value keep same with URDF
        self.declare_parameter("wheel_radius", 0.05)
        self.declare_parameter("wheel_separation_real", 0.34)  # 物理真实值
        self.declare_parameter("wheel_separation_est", 0.34)  # 控制器/里程计使用值

        self.wheel_radius: float = self.get_parameter("wheel_radius").value
        self.wheel_separation_real: float = self.get_parameter(
            "wheel_separation_real"
        ).value
        self.wheel_separation_est: float = self.get_parameter(
            "wheel_separation_est"
        ).value

        # 真实位姿（物理世界）
        self.x_real: float = 0.0
        self.y_real: float = 0.0
        self.theta_real: float = 0.0
        # 估计位姿（里程计）
        self.x_est: float = 0.0
        self.y_est: float = 0.0
        self.theta_est: float = 0.0
        # 轮子转角
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
            f"wheel_separation_real={self.wheel_separation_real}, "
            f"wheel_separation_est={self.wheel_separation_est}"
        )

    def cmd_vel_callback(self, msg: Twist) -> None:
        self.v = msg.linear.x
        self.omega = msg.angular.z

    def update_callback(self) -> None:
        current_time = self.get_clock().now()
        dt = (current_time - self.last_time).nanoseconds / 1e9  # unit: s
        self.last_time = current_time
        if dt <= 0.0:
            return

        # 轮速
        v_left = self.v - self.omega * self.wheel_separation_est / 2.0
        v_right = self.v + self.omega * self.wheel_separation_est / 2.0

        # 真实运动
        v_real = (v_left + v_right) / 2.0
        omega_real = (v_right - v_left) / self.wheel_separation_real
        self.x_real += v_real * math.cos(self.theta_real) * dt
        self.y_real += v_real * math.sin(self.theta_real) * dt
        self.theta_real += omega_real * dt

        # 里程计估计
        v_est = (v_left + v_right) / 2.0
        omega_est = (v_right - v_left) / self.wheel_separation_est
        self.x_est += v_est * math.cos(self.theta_est) * dt
        self.y_est += v_est * math.sin(self.theta_est) * dt
        self.theta_est += omega_est * dt

        self.left_wheel_angle += (v_left / self.wheel_radius) * dt
        self.right_wheel_angle += (v_right / self.wheel_radius) * dt

        self.publish_joint_states(current_time)

        self.publish_odom(current_time)

    def publish_joint_states(self, current_time: Time) -> None:
        msg = JointState()
        msg.header.stamp = current_time.to_msg()
        msg.name = ["base_to_left_wheel", "base_to_right_wheel"]
        msg.position = [self.left_wheel_angle, self.right_wheel_angle]
        self.joint_pub.publish(msg)

    def publish_odom(self, current_time: Time) -> None:
        q_real = Quaternion()
        q_real.x = 0.0
        q_real.y = 0.0
        q_real.z = math.sin(self.theta_real / 2.0)
        q_real.w = math.cos(self.theta_real / 2.0)

        t = TransformStamped()
        t.header.stamp = current_time.to_msg()
        t.header.frame_id = "odom"
        t.child_frame_id = "base_link"
        t.transform.translation.x = self.x_real
        t.transform.translation.y = self.y_real
        t.transform.translation.z = 0.0
        t.transform.rotation = q_real
        self.tf_broadcaster.sendTransform(t)

        q_est = Quaternion()
        q_est.x = 0.0
        q_est.y = 0.0
        q_est.z = math.sin(self.theta_est / 2.0)
        q_est.w = math.cos(self.theta_est / 2.0)

        odom = Odometry()
        odom.header.stamp = current_time.to_msg()
        odom.header.frame_id = "odom"
        odom.child_frame_id = "base_link"
        odom.pose.pose.position.x = self.x_est
        odom.pose.pose.position.y = self.y_est
        odom.pose.pose.position.z = 0.0
        odom.pose.pose.orientation = q_est
        odom.twist.twist.linear.x = self.v
        odom.twist.twist.angular.z = self.omega
        self.odom_pub.publish(odom)


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = DiffDriveController()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
