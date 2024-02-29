#include "stdio.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
// #include "cross_pkg_messages/ManualArmControl.h"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Twist.h"

//#include "sensor_msgs/CompressedImage.h"
#include "sensor_msgs/JointState.h"

ros::Publisher driveTrainPublisher;

void calback(const cross_pkg_messages::RoverComputerDriveCMDConstPtr& msg) {

  auto left = (msg->CMD_L.x + msg->CMD_L.y + msg->CMD_L.z) / 3;
  auto right = (msg->CMD_R.x + msg->CMD_R.y + msg->CMD_R.z) / 3;

  //convert to linear and angular velocity

  auto whealbaseWdith = 0.66;

  auto linear = (left - right);

  auto left_vel = -left * whealbaseWdith / 2;
  auto right_vel = -right * whealbaseWdith / 2;
  auto angular = (right_vel + left_vel);

  geometry_msgs::Twist t{};

  t.linear.x = linear * 1.2;
  t.angular.z = angular * 1.4;




  driveTrainPublisher.publish(t);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "Gazebo_Drive_Link"); 
    ros::NodeHandle n;

    ROS_INFO("Gazebo Drive Link");

   ros::Rate loop_rate(60);

    driveTrainPublisher = n.advertise<geometry_msgs::Twist>("/diff_drive_controller", 9);
    ros::Publisher joinPub = n.advertise<sensor_msgs::JointState>("/joint_states", 10);

    auto driveCommandsSub = n.subscribe("/roverDriveCommands", 10, calback); 


   while (ros::ok()) {
      ros::spinOnce();


      //   sensor_msgs::JointState s;

      //   s.name = {""};
      //   s.position = {0};
      //   s.velocity = {10};
      // //   s.effort = {};

      //   joinPub.publish(s);

        loop_rate.sleep();
   }
}