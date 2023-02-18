#include "stdio.h"

#include "ros/ros.h"
#include <sstream>
#include "cross_pkg_messages/ManualDriveCMD.h"
#include "sensor_msgs/Joy.h"

// #include "cs_libguarded/cs_libguarded.h"

ros::Publisher manualDrive_pub;


// geometry_msgs::Vector3 differentialDriveMapper(double driveAxis, double turnAxis) {

// }

sensor_msgs::JoyConstPtr last0MSG;
sensor_msgs::JoyConstPtr last1MSG;

void joy0Callback(const sensor_msgs::JoyConstPtr& msg) {
   last0MSG = msg;
}
void joy1Callback(const sensor_msgs::JoyConstPtr& msg) {
   last1MSG = msg;
}

void sendCMD() {
   cross_pkg_messages::ManualDriveCMD cmd;
   cmd.value.x = last0MSG->axes[1];
   cmd.value.y = last1MSG->axes[1];
   cmd.value.z = 0; // this is ignored

   // send the command
   manualDrive_pub.publish(cmd);
}

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "JoyMapper"); 

   ros::NodeHandle n;

   ROS_INFO("Joy Mapper is running");
   
   manualDrive_pub = n.advertise<cross_pkg_messages::ManualDriveCMD>("manualCommands", 1000);

   //register callbacks 

   //the queue is set very small so the latest command is always used
   ros::Subscriber sub1 = n.subscribe("joy0", 1000, joy0Callback);
   ros::Subscriber sub2 = n.subscribe("joy1", 1000, joy1Callback);

   ros::spin();

   return 0;
}