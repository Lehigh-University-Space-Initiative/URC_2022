#!/bin/bash

source /opt/ros/melodic/setup.bash
source /home/pi/urc_deploy/devel/setup.bash

export ROS_MASTER_URI=http://192.168.1.3:11311

export ROS_IP=192.168.1.5

# if file of this name exists on disk, don't auto start
if [ -f "/home/pi/urc_deploy/src/pi_nodes/launch/no_auto_start" ] && [ "$1" != "-f" ]; then
    echo "not starting"
else
roslaunch pi_nodes pi.launch
fi