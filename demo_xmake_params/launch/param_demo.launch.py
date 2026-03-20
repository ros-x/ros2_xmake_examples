import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    pkg_share = get_package_share_directory('demo_xmake_params')
    config_file = os.path.join(pkg_share, 'config', 'defaults.yaml')

    return LaunchDescription([
        Node(
            package='demo_xmake_params',
            executable='param_node',
            name='param_node',
            parameters=[config_file],
            output='screen',
        ),
    ])
