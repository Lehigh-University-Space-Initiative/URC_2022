#include "stdio.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
//#include "cross_pkg_messages/RoverComputerDriveCMD.h"
// #include "cross_pkg_messages/ManualArmControl.h"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Twist.h"


int main(int argc, char** argv) {
    ros::init(argc, argv, "Gazebo_Drive_Link"); 
    ros::NodeHandle n;

    ROS_INFO("Gazebo Drive Link");

   ros::Rate loop_rate(100);

    ros::Publisher driveTrainPublisher = n.advertise<geometry_msgs::Twist>("/diff_drive_controller/cmd_vel", 10);

   while (ros::ok()) {
      ros::spinOnce();

        geometry_msgs::Twist t{};
        t.angular.x = 0.5;
        t.angular.y = 0;
        t.angular.z = 0;

        t.linear.x = 0;
        t.linear.y = 0;
        t.linear.z = 0;

        driveTrainPublisher.publish(t);

        loop_rate.sleep();
   }
}