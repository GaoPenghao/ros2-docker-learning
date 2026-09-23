from launch import LaunchDescription
from launch.substitutions import Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description() -> LaunchDescription:
    pkg_share = get_package_share_directory("learning_robot_description")
    # urdf_file = os.path.join(pkg_share, "urdf", "simple_robot.urdf")
    urdf_file = os.path.join(pkg_share, "urdf", "simple_robot.urdf.xacro")
    rviz_file = os.path.join(pkg_share, "rviz", "display.rviz")

    # with open(urdf_file, "r") as f:
    #     robot_description_content = f.read()
    robot_description_content = ParameterValue(
        Command(["xacro ", urdf_file]), value_type=str
    )

    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_description_content}],
        output="screen",
    )

    joint_state_publisher_gui_node = Node(
        package="joint_state_publisher_gui",
        executable="joint_state_publisher_gui",
        # parameters=[{"robot_description": robot_description_content}],
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
