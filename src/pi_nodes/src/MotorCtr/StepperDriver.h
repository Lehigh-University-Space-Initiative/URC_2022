// #pragma once
// #include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <net/if.h>
// #include <sys/ioctl.h>
// #include <sys/socket.h>
// #include <linux/can.h>
// #include <linux/can/raw.h>
#include <array>
#include <vector>
#include <chrono>

#include <pigpio.h>


class StepperDriver {
protected:
    int dirPin;
    int stepPin;
    int enablePin;

    float gearRatio;
    int stepsPerRev;

    bool initialSetup = false;
    void setupGPIO();

    //internal control
    void step();
    void runStepTick();

    //internal relative encoder
    float internalEncoder = 0;

    bool dir = false;

    //internal control

    //velocity in after gear rev/s
    float commandedVelocity = 0;

    //if a new command has not been sent after 0.5 seconds, set velocity to 0
    std::chrono::time_point<std::chrono::system_clock> lastTickTime;
    //last time a step was taken
    std::chrono::time_point<std::chrono::system_clock> lastStepTime;
    
    //the time the last command was sent (used to determain if lost connection)
    std::chrono::time_point<std::chrono::system_clock> lastCommandTime;

    //have a class enum for different control modes
    enum class ControlMode {
        VELOCITY,
        MANUAL
    };

    //TODO: impliment different control modes
    ControlMode controlMode = ControlMode::VELOCITY;


public:
    //Warning: when using a comman enable pin, all steppers must be created before calling setEnable
    StepperDriver(int dirPin, int stepPin, int enablePin, float gearRatio, int stepsPerRev);
    virtual ~StepperDriver();

    //Make sure to handle fact that all stepers will be on the same enable pin
    void setEnable(bool enable);
    bool getEnable();

    void setDir(bool dir);
    bool getDir();

    void setVelocity(float velocity);
    float getVelocity();

    //must be called at a constant rate of 500Hz (every 2ms)
    void tick();
};