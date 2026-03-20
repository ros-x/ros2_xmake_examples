from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='demo_xmake_tracing',
            executable='traced_pipeline',
            name='traced_pipeline',
            output='screen',
        ),
    ])
