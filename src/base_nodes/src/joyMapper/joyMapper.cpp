#include "stdio.h"

#include "ros/ros.h"
#include <sstream>
#include "cross_pkg_messages/ManualDriveCMD.h"
#include "sensor_msgs/Joy.h"

#include "cs_libguarded/cs_libguarded.h"

ros::Publisher manualDrive_pub;

libguarded::plain_guarded<int> x;

void joyCallback(const sensor_msgs::JoyConstPtr& msg) {
   //print out the first 3 axes and the first 3 buttons
   //  ROS_INFO("I heard: [%f]", msg->axes[0]);
   //  ROS_INFO("I heard: [%f]", msg->axes[1]);
   //  ROS_INFO("I heard: [%f]", msg->axes[2]);
   //  ROS_INFO("I heard: [%f]", msg->buttons[0]);
   //  ROS_INFO("I heard: [%f]", msg->buttons[1]);
   //  ROS_INFO("I heard: [%f]", msg->buttons[2]);

    //construct a zeroed ManualDriveCMD
   //  cross_pkg_messages::ManualDriveCMD cmd;
   //  cmd.value.x = 0;
   //  cmd.value.y = 0;
   //  cmd.value.z = 0;
    
    //send the command
   //  manualDrive_pub.publish(cmd);
}

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "JoyMapper"); 

   ros::NodeHandle n;

   ROS_INFO("Joy Mapper is running");
   
   manualDrive_pub = n.advertise<cross_pkg_messages::ManualDriveCMD>("manualCommands", 1000);

   //register callbacks 

   //the queue is set very small so the latest command is always used
   ros::Subscriber sub = n.subscribe("joy0", 1000, joyCallback);

   ros::spin();

   return 0;
}