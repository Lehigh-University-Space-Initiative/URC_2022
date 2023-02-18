#include "stdio.h"

#include "ros/ros.h"
#include <sstream>
#include "cross_pkg_messages/ManualDriveCMD.h"
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
#include "geometry_msgs/Vector3.h"

cross_pkg_messages::RoverComputerDriveCMD currentDriveCommand{};

ros::Publisher driveTrainPublisher{};

void sendDrivePowers() {
   driveTrainPublisher.publish(currentDriveCommand);
}

void manualInputCalback(const cross_pkg_messages::ManualDriveCMDConstPtr& msg) {
   // ROS_INFO("I heard: [%d]", msg->value.x);

   //TODO: convert to pid:
   currentDriveCommand.CMD_L.x = msg->value.x;
   currentDriveCommand.CMD_L.y = msg->value.x;
   currentDriveCommand.CMD_L.z = msg->value.x;

   currentDriveCommand.CMD_R.x = msg->value.y;
   currentDriveCommand.CMD_R.y = msg->value.y;
   currentDriveCommand.CMD_R.z = msg->value.y;

   sendDrivePowers();
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
   driveTrainPublisher = n.advertise<cross_pkg_messages::RoverComputerDriveCMD>("roverDriveCommands", 1);

   while (ros::ok()) {
      //proccsing

      //run loop
      ros::spinOnce();
      loop_rate.sleep();
   }

}