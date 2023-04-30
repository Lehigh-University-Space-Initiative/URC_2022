// #pragma once

#ifndef DriveTrainMotorManager_FILE
#define DriveTrainMotorManager_FILE

#include "CANDriver.h"
// #include "Utilities.h"
#include "ros/ros.h"

#include <cs_plain_guarded.h>

#include <thread>

#include "cross_pkg_messages/RoverComputerDriveCMD.h"

class DriveTrainMotorManager {
private:
    std::vector<SparkMax> motors;

    void setupMotors();
    void stopAllMotors();

    libguarded::plain_guarded<std::chrono::time_point<std::chrono::system_clock>> lastManualCommandTime{std::chrono::system_clock::now()};
    std::chrono::milliseconds manualCommandTimeout{1500};

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
#endif
