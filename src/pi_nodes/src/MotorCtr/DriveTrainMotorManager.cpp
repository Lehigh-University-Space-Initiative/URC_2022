#include "DriveTrainMotorManager.h"


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

    ROS_ERROR("Testing Motors");
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

void DriveTrainMotorManager::parseDriveCommands(const cross_pkg_messages::RoverComputerDriveCMDConstPtr &msg)
{
    ROS_INFO("Drive Commands Recieved with L: %f, R: %f", msg->CMD_L.x, msg->CMD_R.x);
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
