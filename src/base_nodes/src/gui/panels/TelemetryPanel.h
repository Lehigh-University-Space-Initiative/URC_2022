#pragma once
#include "../Panel.h"
#include "../GUI.h"
#include <chrono>
#include <thread>
#include <mutex>

class TelemetryPanel: public Panel {
protected:

    //models:
    cross_pkg_messages::RoverComputerDriveCMD lastDriveCMD;
    cross_pkg_messages::RoverComputerDriveCMD lastArmCMD;

    ros::Subscriber sub;
    ros::Subscriber sub2;

    ros::NodeHandle n;

    virtual void drawBody();
public:
    using Panel::Panel;

    virtual void setup() override;
    virtual void update() override;
    ~TelemetryPanel();
};