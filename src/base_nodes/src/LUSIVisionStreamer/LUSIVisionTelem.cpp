#include "LUSIVisionTelem.h"

// #include <chrono>

std::atomic<int> numClientsConnected{0};

LUSIVIsionGenerator::LUSIVIsionGenerator(ros::NodeHandle n)
{

    auto f = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
        this->lastDriveCMD = *p;
    });
    auto f2 = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
        this->lastArmCMD = *p;
    });
    this->sub = n.subscribe("roverDriveCommands", 10, f);
    this->sub2 = n.subscribe("manualArmControl", 10, f2);

    //gps
    auto f3 = boost::function<void(const cross_pkg_messages::GPSDataConstPtr&)>([this](auto p){
        this->lastGPS = *p;
    });
    this->sub3 = n.subscribe("/gps_data", 10, f3);


}

LUSIVisionTelem LUSIVIsionGenerator::generate()
{
    LUSIVisionTelem telem{};

    telem.softwareInTheLoopTestMode = false;
    telem.controlScheme = 0;
    telem.operationMode = 0;
    //No timestamp data
    // telem.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    telem.driveInputsLeft[0] = lastDriveCMD.CMD_L.x;
    telem.driveInputsLeft[1] = lastDriveCMD.CMD_L.y;
    telem.driveInputsLeft[2] = lastDriveCMD.CMD_L.z;

    telem.driveInputsRight[0] = lastDriveCMD.CMD_R.x;
    telem.driveInputsRight[1] = lastDriveCMD.CMD_R.y;
    telem.driveInputsRight[2] = lastDriveCMD.CMD_R.z;

    telem.armInputs[0] = lastArmCMD.CMD_L.x;
    telem.armInputs[1] = lastArmCMD.CMD_L.y;
    telem.armInputs[2] = lastArmCMD.CMD_L.z;
    telem.armInputs[3] = lastArmCMD.CMD_R.x;
    telem.armInputs[4] = lastArmCMD.CMD_R.y;
    telem.armInputs[5] = lastArmCMD.CMD_R.z;

    telem.numClientsConnected = numClientsConnected.load();

    telem.gpsLLA[0] = lastGPS.lla.x;
    telem.gpsLLA[1] = lastGPS.lla.y;
    telem.gpsLLA[2] = lastGPS.lla.z;

    telem.gpsSatellites = lastGPS.sats;

    telem.course = lastGPS.course;
    telem.speed = lastGPS.speed;


    return telem;
}
