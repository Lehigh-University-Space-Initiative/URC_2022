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
#include "ArmMotorController.h"

#include "StepperDriver.h"

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


//StepperDriver wrist_yaw(10,27,22,1,200);

void callback(const cross_pkg_messages::RoverComputerDriveCMDConstPtr& t) {
   //ROS_INFO("Sending %f",t->CMD_R.z);
   //wrist_yaw.setVelocity(t->CMD_R.z);
}

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "Motor_CTR"); 
   ros::NodeHandle n;

   ROS_INFO("Motor CTR startup");

   //DriveTrainMotorManager driveTrain{};
   ArmMotorManager arm{};

   ros::Rate loop_rate(800);

   auto driveCommandsSub = n.subscribe("/roverDriveCommands", 10, callback); 
   

   //wrist_yaw.setEnable(false);
   //wrist_yaw.setDir(true);

   while (ros::ok()) {
      ros::spinOnce();

      arm.tick();

      //wrist_yaw.tick();

      loop_rate.sleep();
   }

   //wrist_yaw.setEnable(true);
}
