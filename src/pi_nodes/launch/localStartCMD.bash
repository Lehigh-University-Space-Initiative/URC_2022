#!/bin/bash

source /opt/ros/melodic/setup.bash
source /home/pi/urc_deploy/devel/setup.bash

export ROS_MASTER_URI=http://192.168.1.3:11311

export ROS_IP=192.168.1.5

# Setup CAN

echo lusi | sudo -S ip link set can0 up type can bitrate 1000000
echo lusi | sudo -S ip link set can1 up type can bitrate 1000000
echo lusi | sudo -S ifconfig can0 txqueuelen 65536
echo lusi | sudo -S ifconfig can1 txqueuelen 65536

# if file of this name exists on disk, don't auto start
if [ -f "/home/pi/urc_deploy/src/pi_nodes/launch/no_auto_start" ] && [ "$1" != "-f" ]; then
    echo "not starting"
else
roslaunch pi_nodes pi.launch
fi
