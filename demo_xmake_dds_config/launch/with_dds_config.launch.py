import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable, LogInfo
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    pkg_share = get_package_share_directory('demo_xmake_dds_config')

    rmw_arg = DeclareLaunchArgument(
        'rmw_implementation',
        default_value='rmw_fastrtps_cpp',
        description='RMW implementation to use'
    )

    fastdds_config = os.path.join(pkg_share, 'config', 'fastdds_profile.xml')

    return LaunchDescription([
        rmw_arg,
        SetEnvironmentVariable('RMW_IMPLEMENTATION', LaunchConfiguration('rmw_implementation')),
        SetEnvironmentVariable('FASTRTPS_DEFAULT_PROFILES_FILE', fastdds_config),
        LogInfo(msg=['demo_xmake_dds_config: RMW=', LaunchConfiguration('rmw_implementation')]),
        LogInfo(msg=['demo_xmake_dds_config: FastDDS profile at ', fastdds_config]),
    ])
