#!/bin/bash

source /opt/ros/melodic/setup.bash
source ~/URC_2022/devel_cb/setup.bash

export ROS_MASTER_URI=http://192.168.1.3:11311
export ROS_IP=192.168.1.250

roslaunch base_nodes baseStation.launch