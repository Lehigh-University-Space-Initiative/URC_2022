
#include "stdio.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
#include "geometry_msgs/Vector3.h"
#include "CANDriver.h"

//drive motors
PWMSparkMax leftSide(18);
// PWMSparkMax rightSide(18);


void gotCommands(const cross_pkg_messages::RoverComputerDriveCMD& msg) {
   ROS_INFO(("setting power to " + std::to_string(msg.CMD_L.x)).c_str());
   leftSide.setPower(msg.CMD_L.x);
   //do right
}

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "Motor_CTR"); 

   ros::NodeHandle n;

   ros::Subscriber sub = n.subscribe("roverDriveCommands", 1000, gotCommands);

   ROS_INFO("Motor CTR startup");

   // ros::spinOnce();

   // while (ros::ok) {
      
   // }

   // ros::spin();

   // ROS_INFO("testing cont");
   // SparkMax con(0,2);

   // con.sendPowerCMD(0.1);
   // sleep(1);
   // con.sendPowerCMD(0.0);

   ros::spin();

   //dealloc

   CANDriver::closeCAN();
}