
#include "stdio.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
// #include "cross_pkg_messages/ManualArmControl.h"
#include "geometry_msgs/Vector3.h"
#include "CANDriver.h"
#include "StepperDriver.h"

//0 = drivetrain wiring and control, 1 = robotic arm wiring and control
#define WIRING_CONFIG 1

#if !WIRING_CONFIG
//drive motors
PWMSparkMax leftSide(18);
PWMSparkMax rightSide(19);
// PWMSparkMax rightSide2(13);
// PWMSparkMax rightSide3(19);
#else
//4 stepper motors
/* table of motor pins
14			Stepper Ennable
15			Base Rot Dir
23			Base Rot Step
24			Elbow Pitch Dir
25			Elbow Pitch Step
8			Elbow Rotate Dir
7			Elbow Rotate Step
5			End effector Rot Dir
6			End effector Rot Step
*/

StepperDriver BaseRot(15,23,14,1/3.0,200);
StepperDriver ElbowPitch(24,25,14,1/50.0,200);
StepperDriver ElbowRotate(8,7,14,1/15.0,200);
StepperDriver EndEffectorRot(5,6,14,1/27.0,200);


//arm motors


#endif

void gotCommands(const cross_pkg_messages::RoverComputerDriveCMD& msg) {
   ROS_INFO(("setting power to " + std::to_string(msg.CMD_L.x)).c_str());
#if !WIRING_CONFIG
   leftSide.setPower(msg.CMD_L.x);
   rightSide.setPower(-msg.CMD_R.x);
#endif
}

//same msg (but should be manual arm control) but different interpretation
void gotArmCommands(const cross_pkg_messages::RoverComputerDriveCMD& msg) {
   ROS_INFO(("setting power to " + std::to_string(msg.CMD_L.x)).c_str());
#if WIRING_CONFIG
   /* table of motor (ignor if dur or step) to input control
   Base Rot Dir	
   Base Rot Step		L roll
   Elbow Pitch Dir		
   Elbow Pitch Step		R Pitch
   Elbow Rotate Dir	
   Elbow Rotate Step		R Roll
   End effector Rot Dir
   End effector Rot Step		R Yaw
   */

   //use the table above to set motor velocities from msg
   //0=pitch, 1=yaw, 2=roll

   float stepperVelocitySensitivity = 0.1;
   //pitch
   ElbowPitch.setVelocity(msg.CMD_R.x * stepperVelocitySensitivity);
   //yaw
   EndEffectorRot.setVelocity(msg.CMD_R.y * stepperVelocitySensitivity);
   //roll
   BaseRot.setVelocity(msg.CMD_L.z * stepperVelocitySensitivity);
   ElbowRotate.setVelocity(msg.CMD_R.z * stepperVelocitySensitivity);

#endif
}

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "Motor_CTR"); 

   ros::NodeHandle n;

   ros::Subscriber sub = n.subscribe("roverDriveCommands", 1000, gotCommands);
   ros::Subscriber sub2 = n.subscribe("manualArmControl", 1000, gotArmCommands);

   ROS_INFO("Motor CTR startup");

   //TODO: ennable steppers

   // ros::spinOnce();

   // while (ros::ok) {
      
   // }

   // ros::spin();

   // ROS_INFO("testing cont");
   // SparkMax con(0,2);

   // con.sendPowerCMD(0.1);
   // sleep(1);
   // con.sendPowerCMD(0.0);

   // leftSide.setPower(0.1);
   // rightSide.setPower(0.1);

   ros::spin();

   //dealloc

   CANDriver::closeCAN();
}