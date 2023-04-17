#pragma once

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
