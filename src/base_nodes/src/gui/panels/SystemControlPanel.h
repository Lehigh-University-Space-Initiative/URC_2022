#pragma once
#include "../Panel.h"
#include "../GUI.h"
#include <chrono>
#include <thread>
#include <mutex>

class SystemControlPanel: public Panel {
protected:

    //models:
    // cross_pkg_messages::RoverComputerDriveCMD lastDriveCMD;

    ros::Subscriber sub;

    ros::NodeHandle n;

    virtual void drawBody();
public:
    using Panel::Panel;

    virtual void setup() override;
    virtual void update() override;
    ~SystemControlPanel();
};