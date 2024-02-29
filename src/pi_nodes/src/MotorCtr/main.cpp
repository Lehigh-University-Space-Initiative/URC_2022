#include "stdio.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
// #include "cross_pkg_messages/ManualArmControl.h"
#include "geometry_msgs/Vector3.h"
#include "CANDriver.h"
#include "StepperDriver.h"
#include "DriveTrainMotorManager.h"


/*
Note 
Left stick:
pitch: pitch
roll: base rotate
yaw: M3 rist pitch

right stick
pitch: elbow pitch
roll: rist roll
yaw: rist/ yaw

left trigger: close end effect
right trigger: open end effect
*/


int main(int argc, char** argv) {
    
   ros::init(argc, argv, "Motor_CTR"); 
   ros::NodeHandle n;

   ROS_INFO("Motor CTR startup");

   DriveTrainMotorManager driveTrain{};

   ros::Rate loop_rate(100);

   while (ros::ok()) {
      ros::spinOnce();
      loop_rate.sleep();
   }
}
