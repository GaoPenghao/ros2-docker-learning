from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os
import subprocess


def generate_launch_description() -> LaunchDescription:
    pkg_share = get_package_share_directory("learning_robot_description")
    urdf_file = os.path.join(pkg_share, "urdf", "mobile_robot.urdf.xacro")
    rviz_file = os.path.join(pkg_share, "rviz", "display.rviz")

    # 在 launch 初始化阶段用 xacro 展开一次，避免多次执行
    result = subprocess.run(
        ["xacro", urdf_file], capture_output=True, text=True, check=True
    )
    robot_description_content = result.stdout

    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_description_content}],
        output="screen",
    )

    joint_state_publisher_gui_node = Node(
        package="joint_state_publisher_gui",
        executable="joint_state_publisher_gui",
        output="screen",
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", rviz_file],
        output="screen",
    )

    return LaunchDescription(
        [
            robot_state_publisher_node,
            joint_state_publisher_gui_node,
            rviz_node,
        ]
    )
