#!/bin/bash

source /opt/ros/melodic/setup.bash
source /home/michael/URC_2022/devel_cb/setup.bash

export ROS_MASTER_URI=http://192.168.1.3:11311
export ROS_IP=192.168.1.250

# if file of this name exists on disk, don't auto start
# if [ -f "/home/jimmy/urc_deploy/src/jetson_nodes/launch/no_auto_start" ] && [ "$1" != "-f" ]; then
#     echo "not starting"
# else
    roslaunch base_nodes baseStation.launch
# fi