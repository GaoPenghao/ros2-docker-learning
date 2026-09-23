from setuptools import find_packages, setup

package_name = 'py_tf2'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='zy',
    maintainer_email='gph1208@163.com',
    description='TODO: Package description',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'static_broadcaster = py_tf2.static_broadcaster:main',
            'dynamic_broadcaster = py_tf2.dynamic_broadcaster:main',
            'tf2_listener = py_tf2.tf2_listener:main',
        ],
    },
)
