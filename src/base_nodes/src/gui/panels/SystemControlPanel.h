#pragma once
#include "../Panel.h"
#include "../GUI.h"
#include <chrono>
#include <thread>
#include <mutex>

class SystemControlPanel: public Panel {
    public:
        using Panel::Panel;

        virtual void setup() override;
        virtual void update() override;
        ~SystemControlPanel();
    
    protected:
        //Variables:
        bool eStopStatus = true; //Button initially set to red and e-stop initially on for safety
        
        //Models:
        // cross_pkg_messages::RoverComputerDriveCMD lastDriveCMD;
        ros::Subscriber sub;
        ros::NodeHandle n;
        virtual void drawBody();
};