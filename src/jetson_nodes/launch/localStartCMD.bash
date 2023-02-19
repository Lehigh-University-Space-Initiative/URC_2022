#!/bin/bash

source /opt/ros/melodic/setup.bash
source /home/jimmy/urc_deploy/devel/setup.bash

export ROS_IP=192.168.1.3

# if file of this name exists on disk, don't auto start
if [ -f "/home/jimmy/urc_deploy/src/jetson_nodes/launch/no_auto_start" ] && [ "$1" != "-f" ]; then
    echo "not starting"
else
    roslaunch jetson_nodes jetson.launch
fi