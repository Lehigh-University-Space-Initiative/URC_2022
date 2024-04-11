#pragma once


#include "StepperDriver.h"
#include "CANDriver.h"
// #include "Utilities.h"
#include "ros/ros.h"

#include <cs_plain_guarded.h>

#include <thread>

#include "cross_pkg_messages/RoverComputerDriveCMD.h"

class ArmMotorManager {
private:
    std::vector<StepperDriver> motors;

    void setupMotors();
    void stopAllMotors();

    libguarded::plain_guarded<std::chrono::time_point<std::chrono::system_clock>> lastManualCommandTime{std::chrono::system_clock::now()};
    std::chrono::milliseconds manualCommandTimeout{1500};

    ros::Subscriber armCommandsSub;
    ros::NodeHandle nodeHandle;

    void parseDriveCommands(const cross_pkg_messages::RoverComputerDriveCMDConstPtr& msg);

    // dir , step, enable
    StepperDriver elbow_pitch = StepperDriver(5,6,22,5.0,200);
    StepperDriver shoulder_pitch = StepperDriver(21,20,22,1.0,200);

    StepperDriver wrist_yaw = StepperDriver(11,4,22,1/100.0,200);
    StepperDriver wrist_roll = StepperDriver(17,27,22,1/30.0,200);
    StepperDriver wrist_pitch = StepperDriver(10,9,22,1/5.0,200);



    //PWMSparkMax end_effector = PWMSparkMax(13);

public:
    ArmMotorManager();
    virtual ~ArmMotorManager();

    void tick();
};