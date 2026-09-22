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

    namespace_arg = DeclareLaunchArgument(
        "namespace", default_value="robot1", description="Namespace for the node"
    )

    subscriber_node = Node(
        package="py_msg",
        executable="robot_status_subscriber",
        name="robot_status_subscriber",
        namespace=LaunchConfiguration("namespace"),
        parameters=[config_file],
        output="screen",
    )

    return LaunchDescription(
        [
            namespace_arg,
            subscriber_node,
        ]
    )
