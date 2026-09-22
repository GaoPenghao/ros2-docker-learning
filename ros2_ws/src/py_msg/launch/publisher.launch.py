from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description() -> LaunchDescription:
    config_file = os.path.join(
        get_package_share_directory("py_msg"), "config", "robot_params.yaml"
    )

    robot_name_arg = DeclareLaunchArgument(
        "robot_name", default_value="launch_bot", description="Name of the robot"
    )

    namespace_arg = DeclareLaunchArgument(
        "namespace", default_value="robot1", description="Namespace for the node"
    )

    publisher_node = Node(
        package="py_msg",
        executable="robot_status_publisher",
        name="robot_status_publisher",
        namespace=LaunchConfiguration("namespace"),
        parameters=[
            config_file,
            {"robot_name": LaunchConfiguration("robot_name")},
        ],
        output="screen",
    )

    return LaunchDescription(
        [
            robot_name_arg,
            namespace_arg,
            publisher_node,
        ]
    )
