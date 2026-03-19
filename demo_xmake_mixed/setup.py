from setuptools import setup

package_name = 'demo_xmake_mixed'

setup(
    name=package_name,
    version='0.1.0',
    packages=[package_name],
    install_requires=['setuptools'],
    entry_points={
        'console_scripts': [
            'py_subscriber = demo_xmake_mixed.py_node:main',
        ],
    },
)
