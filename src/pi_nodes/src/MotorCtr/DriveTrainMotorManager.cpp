// #pragma once


#include "CANDriver.h"
#include "Utilities.h"
#include "ros/ros.h"

#include <cs_plain_guarded.h>

#include <thread>

#include "cross_pkg_messages/RoverComputerDriveCMD.h"

class DriveTrainMotorManager {
private:
    std::vector<SparkMax> motors;

    void setupMotors();

    libguarded::plain_guarded<bool> sendHeartbeatsFlag{true};
    libguarded::plain_guarded<bool> heartbeatThreadShutDown{false};
    void sendHeartbeats();
    void heartbeatThread();
    std::thread heartbeatThreadObj;

    ros::Subscriber driveCommandsSub;
    ros::NodeHandle nodeHandle;

    void parseDriveCommands(const cross_pkg_messages::RoverComputerDriveCMDConstPtr& msg);

public:
    DriveTrainMotorManager();
    virtual ~DriveTrainMotorManager();
};

// #include "DriveTrainMotorManager.h"


void DriveTrainMotorManager::setupMotors()
{
    //add motors to the motors vector

    // //Actual Numbers
    {
        //left side (BUS 0)
        motors.push_back(SparkMax(0, 1)); //LF
        motors.push_back(SparkMax(0, 2)); //LM
        motors.push_back(SparkMax(0, 3)); // LB
    }
    {
        //right side (BUS 1
        motors.push_back(SparkMax(0, 4)); //RB
        motors.push_back(SparkMax(0, 5)); //RM
        motors.push_back(SparkMax(0, 6)); //RF
    }

// //     //Testing numbers
// //     motors.push_back(SparkMax(0,15));
// //     ROS_INFO("Identifying motor 0");
// //     motors[0].ident();
    ROS_ERROR("TEST ERR");
    for (auto& motor: motors)
        motor.ident();
}

void DriveTrainMotorManager::sendHeartbeats()
{
    for (auto& motor : motors) {
        motor.sendHeartbeat();
    }
}

void DriveTrainMotorManager::heartbeatThread()
{
    ros::Rate loop_rate(50);
    while(true)
    {
        { // lock block
            //if we are shutting down, exit the thread
            auto lock = heartbeatThreadShutDown.lock();
            if (*lock) {
                return;
            }
        }
        { // lock block
            //if we are sending heartbeats, send thevim.wo.relativenumber = truem
            auto lock = sendHeartbeatsFlag.lock();
            if (*lock) {
                // ROS_INFO("Sending Heartbeats");
                sendHeartbeats();
            }
        }
        loop_rate.sleep();
    }
}

void DriveTrainMotorManager::parseDriveCommands(const cross_pkg_messages::RoverComputerDriveCMDConstPtr &msg) {
   ROS_INFO("Drive Commands Recieved with L: %f, R: %f", msg->CMD_L.x, msg->CMD_R.x);
   
   // Set motor commands to 0 if eStopStatus is true
   bool eStopStatus;
   ros::param::get("eStopStatus", eStopStatus);
   
   if (eStopStatus) {
      currentDriveCommand.CMD_L.x = 0;
      currentDriveCommand.CMD_L.y = 0;
      currentDriveCommand.CMD_L.z = 0;
      currentDriveCommand.CMD_R.x = 0;
      currentDriveCommand.CMD_R.y = 0;
      currentDriveCommand.CMD_R.z = 0;
   }
    
    //testing: send the left power to all motors

    // for (auto& motor : motors) {
    //     motor.sendPowerCMD(msg->CMD_L.x);
    // }

    motors[0].sendPowerCMD(msg->CMD_L.x);
    motors[1].sendPowerCMD(msg->CMD_L.y);
    motors[2].sendPowerCMD(msg->CMD_L.z);

    motors[3].sendPowerCMD(msg->CMD_R.x);
    motors[4].sendPowerCMD(msg->CMD_R.y);
    motors[5].sendPowerCMD(msg->CMD_R.z);
}

DriveTrainMotorManager::DriveTrainMotorManager()
{
    setupMotors();

    //start heartbeat thread
    heartbeatThreadObj = std::thread(&DriveTrainMotorManager::heartbeatThread, this);   

    auto calback = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
        parseDriveCommands(p);
    });

    //subscribe to drive commands
    driveCommandsSub = nodeHandle.subscribe("/roverDriveCommands", 10, calback); 
} 

DriveTrainMotorManager::~DriveTrainMotorManager()
{
    //tell heartbeat thread to shut down
    {
        auto lock = heartbeatThreadShutDown.lock();
        *lock = true;
    }

    //wait for heartbeat thread to shut down
    heartbeatThreadObj.join();

}
