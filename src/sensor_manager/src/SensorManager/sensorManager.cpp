#include "ros/ros.h"

int main(int argc, char** argv){
    ros::init(argc, argv, "SensorManager");

    ROS_INFO("SensorManager startup");
    ros::NodeHandle n;

    ros::spin();

    

    ros::Rate loop_rate(10);
}