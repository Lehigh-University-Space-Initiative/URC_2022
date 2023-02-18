
#include "stdio.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
#include "geometry_msgs/Vector3.h"
#include "CANDriver.h"

void gotCommands(const cross_pkg_messages::RoverComputerDriveCMD& msg) {

}

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "Motor_CTR"); 

   ros::NodeHandle n;

   // ros::Subscriber sub = n.subscribe("roverDriveCommands", 1, gotCommands);

   ROS_INFO("HEllo there");

   ros::spinOnce();

   // while (ros::ok) {
      
   // }

   // ros::spin();

   // ROS_INFO("testing cont");
   // SparkMax con(0,2);

   // con.sendPowerCMD(0.1);
   // sleep(1);
   // con.sendPowerCMD(0.0);

   // ros::spinOnce();

   ROS_INFO("testing pwm cont");

   PWMSparkMax pwm(18);

   //wait 1 second, spin at 0.2 power wait 1 second, spin at 0.0 power
   sleep(1);
   pwm.setPower(0.2);
   ROS_INFO("spinning");
   sleep(100);
   pwm.setPower(0.0);
   ROS_INFO("spinning at 0.0");

   //dealloc

   // CANDriver::closeCAN();
   PWMSparkMax::terminateGPIO();
}