from typing import Optional

import rclpy
from rclpy.node import Node
from rcl_interfaces.msg import Parameter, SetParametersResult
from tutorial_interfaces.msg import RobotStatus


class RobotStatusPublisher(Node):
    def __init__(self) -> None:
        super().__init__("robot_status_publisher")

        # 1. 声明参数
        self.declare_parameter("robot_name", "learning_bot")
        self.declare_parameter("initial_battery", 100.0)
        self.declare_parameter("publish_period", 1.0)

        # 2. 读取参数值
        self.robot_name: str = self.get_parameter("robot_name").value
        self.init_battery: float = self.get_parameter("initial_battery").value
        publish_period: float = self.get_parameter("publish_period").value

        self.add_on_set_parameters_callback(self.parameters_callback)

        self.publisher = self.create_publisher(RobotStatus, "robot_status", 10)
        self.timer = self.create_timer(publish_period, self.timer_callback)
        self.battery_level = self.init_battery

    def timer_callback(self) -> None:
        msg = RobotStatus()
        msg.robot_name = self.robot_name
        msg.battery_level = self.battery_level
        msg.is_active = True
        msg.joint_positions = [0.0, 0.5, -0.5, 1.0, -1.0]

        self.publisher.publish(msg=msg)
        self.get_logger().info(
            f"Publishing: robot_name={msg.robot_name}, "
            f"battery={msg.battery_level:.1f}, "
            f"joints={msg.joint_positions}"
        )

        self.battery_level -= 1.0
        if self.battery_level < 0.0:
            self.battery_level = self.init_battery

    def parameters_callback(self, params: list[Parameter]) -> SetParametersResult:
        for param in params:
            if param.name == "robot_name":
                self.robot_name = param.value
                self.get_logger().info(f"robot_name updated to: {param.value}")
        return SetParametersResult(successful=True)


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = RobotStatusPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__name__":
    main()
