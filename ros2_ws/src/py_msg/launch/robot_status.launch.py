from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description() -> LaunchDescription:
    pkg_share = get_package_share_directory("py_msg")

    # 声明 launch 参数
    robot_name_arg = DeclareLaunchArgument(
        "robot_name", default_value="launch_bot", description="Name of the robot"
    )

    # 引用两个子 launch 文件，分别传入不同命名空间
    robot1_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_share, "launch", "publisher.launch.py")
        ),
        launch_arguments={
            "namespace": "robot1",
            "robot_name": LaunchConfiguration("robot_name"),
        }.items(),
    )

    robot2_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_share, "launch", "publisher.launch.py")
        ),
        launch_arguments={
            "namespace": "robot2",
            "robot_name": "bot_2",
        }.items(),
    )

    subscriber1_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_share, "launch", "subscriber.launch.py")
        ),
        launch_arguments={"namespace": "robot1"}.items(),
    )

    subscriber2_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_share, "launch", "subscriber.launch.py")
        ),
        launch_arguments={"namespace": "robot2"}.items(),
    )

    return LaunchDescription(
        [
            robot_name_arg,
            robot1_launch,
            robot2_launch,
            subscriber1_launch,
            subscriber2_launch,
        ]
    )
