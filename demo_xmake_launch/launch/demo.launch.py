import os
from launch import LaunchDescription
from launch.actions import LogInfo
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    pkg_share = get_package_share_directory('demo_xmake_launch')
    config_file = os.path.join(pkg_share, 'config', 'params.yaml')

    return LaunchDescription([
        LogInfo(msg=['demo_xmake_launch: config file at ', config_file]),
        LogInfo(msg=['demo_xmake_launch: launch file working!']),
    ])
