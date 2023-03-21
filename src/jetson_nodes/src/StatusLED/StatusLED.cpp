#include "ros/ros.h"
// #include <jetsonGPIO.h>

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "StatusLED"); 

    ROS_INFO("StatusLED startup");

    ros::NodeHandle n;

    ros::Rate loop_rate(10);

    int status = 0;
    //if at waypoint this is used to blink light
    bool lightOn = false;

    while (ros::ok()) {

        //check paramter value of /roverStatus
        //this is an int which could have the following values:
        //0 - error
        //1 - manual
        //2 - autonomous
        //3 - autonomous at waypoint
        //if the paramater is not set, set it to 0 (error)

        if (n.getParam("/roverStatus", status)) {
            //param exists
            ROS_INFO("Status: %d", status);
        } else {
            //param does not exist
            ROS_WARN("Status not set, setting to error");
            n.setParam("/roverStatus", 0);
        }

        //the light colors: red autonomus, blue teleop, flashing green on suc arrival 


        ros::spinOnce();
        loop_rate.sleep();
    }

}