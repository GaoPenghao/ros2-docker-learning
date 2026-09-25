import subprocess

from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description() -> LaunchDescription:
    pkg_share = get_package_share_directory("learning_robot")
    urdf_file = os.path.join(pkg_share, "urdf", "mobile_robot.urdf.xacro")
    rviz_file = os.path.join(pkg_share, "rviz", "gazebo.rviz")
    world_file = os.path.join(pkg_share, "worlds", "lidar_world.sdf")

    result = subprocess.run(
        ["xacro", urdf_file], capture_output=True, text=True, check=True
    )
    robot_description_content = result.stdout

    # 1. 启动 Gazebo（-r 表示启动后立即运行，不停在暂停状态）
    gz_sim = ExecuteProcess(
        cmd=["gz", "sim", "-r", "--headless-rendering", world_file], output="screen"
    )

    # 2. 启动 robot_state_publisher，发布 URDF 到 /robot_description
    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[
            {"robot_description": robot_description_content},
            {"use_sim_time": True},
        ],
        output="screen",
    )

    # 3. 把机器人 spawn 到 Gazebo
    spawn_robot = Node(
        package="ros_gz_sim",
        executable="create",
        arguments=[
            "-topic",
            "robot_description",
            "-name",
            "mobile_robot",
            "-z",
            "0.1",
        ],
        output="screen",
    )

    # 4. 桥接 ROS 2 和 Gazebo 话题
    bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        arguments=[
            "/scan@sensor_msgs/msg/LaserScan@gz.msgs.LaserScan",
            "/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock",
        ],
        output="screen",
    )

    # 5. controller spawner: joint_state_broadcaster
    spawn_jsb = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster"],
        output="screen",
    )

    # 6. controller spawner: diff_drive_controller
    spawn_ddc = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["diff_drive_controller"],
        output="screen",
    )

    topic_relay = Node(
        package="py_mobile_robot",
        executable="topic_relay",
        output="screen",
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", rviz_file],
        parameters=[{"use_sim_time": True}],
        output="screen",
    )

    return LaunchDescription(
        [
            gz_sim,
            robot_state_publisher_node,
            spawn_robot,
            bridge,
            spawn_jsb,
            spawn_ddc,
            topic_relay,
            rviz_node,
        ]
    )
