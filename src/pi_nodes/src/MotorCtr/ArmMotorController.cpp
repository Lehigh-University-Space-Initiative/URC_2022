#include "ArmMotorController.h"

void ArmMotorManager::setupMotors()
{
}

void ArmMotorManager::stopAllMotors()
{
}

void ArmMotorManager::parseDriveCommands(const cross_pkg_messages::RoverComputerDriveCMDConstPtr &msg)
{
    //end_effector.setPower(msg->CMD_R.x);
    wrist_yaw.setVelocity(msg->CMD_R.x);
}

ArmMotorManager::ArmMotorManager()
{
    auto calback = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
        {
            auto lock = lastManualCommandTime.lock();
            *lock = std::chrono::system_clock::now();
        }
        parseDriveCommands(p);
    });

    //subscribe to drive commands
    armCommandsSub = nodeHandle.subscribe("/manualArmControl", 10, calback); 

    wrist_yaw.setEnable(true);
}

ArmMotorManager::~ArmMotorManager()
{

}

void ArmMotorManager::tick()
{

   wrist_yaw.tick(); 

}
