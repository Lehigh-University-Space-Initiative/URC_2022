# URC_2022

[Full Docs](https://docs.google.com/document/d/1HZW7jHYWGAuONX8vwVCPRpkgOKioc5_Ffjo8CsBJvVM/edit?usp=sharing)

## Build Instructions

use `catkin build [package name] [package 2 name] ...` to build a package or `catkin build` to build everything

https://catkin-tools.readthedocs.io/en/latest/verbs/catkin_build.html

## Other

[Ros Tutorials](https://wiki.ros.org/ROS/Tutorials)

## Running
 during testing on one computer this command will start the full system: 
 `roslaunch base_nodes fullSystemSingle.launch debug:=true`

# Deploying

use the Urc_DeployTools/deploy.sh script to copy, compile and optionally reboot the rover computers with the new source code

the computers should be setup to run their respective {compName}.launch file upon startup