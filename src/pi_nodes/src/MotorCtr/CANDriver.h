#include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <array>
#include <vector>

#include <pigpio.h>

class CANDriver {
protected:
    int canBus;

    //can stuff
    // static std::array<can_frame,2> frame;
    static std::array<ifreq, 2> ifr;
    static std::array<sockaddr_can, 2> socketAddress;
    static std::array<int,2> soc;

    static std::array<bool,2> canBussesSetup;

    static bool setupCAN(int canBus);
    static bool sendMSG(int canBus, const can_frame& frame);

public:
    CANDriver(int busNum);
    virtual ~CANDriver();

    static void closeCAN();
};

class SparkMax: CANDriver {
protected:
    int canID;
public:
    SparkMax(int canBUS, int canID);

    void sendPowerCMD(float power);
};

class PWMSparkMax {
protected:
    int pin;

    static bool gpioSetup;
    static void setupGPIO();

public:
    static void terminateGPIO();

    //must be a pwm pin
    PWMSparkMax(int pin);
    ~PWMSparkMax();

    void setPower(float power);
};
