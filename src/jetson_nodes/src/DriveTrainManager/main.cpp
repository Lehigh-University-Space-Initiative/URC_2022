#include "stdio.h"

#include "ros/ros.h"
#include <sstream>
#include "cross_pkg_messages/ManualDriveCMD.h"

void manualInputCalback(const cross_pkg_messages::ManualDriveCMDConstPtr& msg) {
   ROS_INFO("I heard: [%d]", msg->value.x);
}

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "DriverTrainManager"); 

   ros::NodeHandle n;

   ROS_INFO("DriverTrainManager is running");
   
   //TOOO: set rate to correct amount
   ros::Rate loop_rate(10); 

   //register callbacks 

   //the queue is set very small so the latest command is always used
   ros::Subscriber sub = n.subscribe("manualCommands", 1, manualInputCalback);

   while (ros::ok()) {
      //proccsing

      //run loop
      ros::spinOnce();
      loop_rate.sleep();
   }

}