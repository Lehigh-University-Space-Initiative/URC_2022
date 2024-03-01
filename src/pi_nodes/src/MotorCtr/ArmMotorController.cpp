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
    wrist_pitch.setVelocity(msg->CMD_L.z);

    //elbow_pitch.setVelocity(msg->CMD_R.x)
    wrist_roll.setVelocity(-msg->CMD_R.y);
    wrist_yaw.setVelocity(msg->CMD_R.z);

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
    wrist_yaw.setEnable(false);
}

void ArmMotorManager::tick()
{

   wrist_yaw.tick(); 
   wrist_roll.tick();
   wrist_pitch.tick();
   elbow_pitch.tick();

}
