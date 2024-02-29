#pragma once
#include "stdio.h"

#include "ros/ros.h"
#include <sstream>

#include <opencv2/opencv.hpp>
#include "sensor_msgs/image_encodings.h"
#include "sensor_msgs/CompressedImage.h"

#include <cv_bridge/cv_bridge.h>

#include "sockpp/udp_socket.h"
#include "sockpp/tcp_acceptor.h"

#include "cs_plain_guarded.h"
#include "thread"


#include "cross_pkg_messages/RoverComputerDriveCMD.h"
#include "cross_pkg_messages/GPSData.h"

#include <atomic>

//atomic count of current number of clients connected to the rover
extern std::atomic<int> numClientsConnected;

struct LUSIVisionTelem {

    uint64_t timestamp;

    //bool softwareInTheLoopTestMode;

    int controlScheme; // 0 = driveline, 1 = arm
    int operationMode; // 0 = manual, 1 = autonomous
    
    int numClientsConnected; // number of LUSI VISION clients connected to the rover

    float driveInputsLeft[3]; // x, y, z
    float driveInputsRight[3]; // x, y, z

    float armInputs[6]; // x, y, z, roll, pitch, yaw

    float gpsLLA[3]; // latitude, longitude, altitude
    float course;
    float speed;
    int gpsSatellites; // number of satellites


};

class LUSIVIsionGenerator {
public:

    cross_pkg_messages::RoverComputerDriveCMD lastDriveCMD{};
    cross_pkg_messages::RoverComputerDriveCMD lastArmCMD{};
    cross_pkg_messages::GPSData lastGPS{};

    ros::Subscriber sub;
    ros::Subscriber sub2;
    ros::Subscriber sub3;

    LUSIVIsionGenerator(ros::NodeHandle n);

    LUSIVisionTelem generate();
};