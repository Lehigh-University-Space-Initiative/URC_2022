#include "DriveTrainMotorManager.h"
#include <thread>


void DriveTrainMotorManager::setupMotors()
{
    //add motors to the motors vector

    /* 
    //Actual Numbers
    {
        //left side (BUS 0)
        motors.push_back(SparkMax(0, 1));
        motors.push_back(SparkMax(0, 2));
        motors.push_back(SparkMax(0, 3));
    }

    {
        //right side (BUS 1
        motors.push_back(SparkMax(1, 4));
        motors.push_back(SparkMax(1, 5));
        motors.push_back(SparkMax(1, 6));
    }
    */

   //Testing numbers
   motors.push_back(SparkMax(0,15));
   motors[0].ident();
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
            //if we are sending heartbeats, send them
            auto lock = sendHeartbeatsFlag.lock();
            if (*lock) {
                sendHeartbeats();
            }
        }

        loop_rate.sleep();
    }
}

void DriveTrainMotorManager::parseDriveCommands(const cross_pkg_messages::RoverComputerDriveCMDConstPtr &msg)
{
    //testing: send the left power to all motors
    for (auto& motor : motors) {
        motor.sendPowerCMD(msg->CMD_L.x);
    }
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
    driveCommandsSub = nodeHandle.subscribe("drive_commands", 10, calback); 
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
