#!/bin/bash

source /opt/ros/melodic/setup.bash
source ~/URC_2022/devel_cb/setup.bash

export ROS_MASTER_URI=http://10.0.0.20:11311
export ROS_IP=10.1.1.41

roslaunch base_nodes baseStation.launch