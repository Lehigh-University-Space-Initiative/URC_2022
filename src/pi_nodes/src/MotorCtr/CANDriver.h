
#ifndef CANDriver_FILE
#define CANDriver_FILE
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
#include "cs_plain_guarded.h"

class CANDriver {
protected:
    int canBus;

    struct CANStaticData {
        ifreq ifr;
        sockaddr_can socketAddress;
        int soc;

        bool canBussesSetup;
    };
    typedef libguarded::plain_guarded<CANStaticData> CANStaticDataGuarded;

    static std::array<CANStaticDataGuarded,2> canStaticData;

    //this allows for ARC allocation to be used on the can busses
    static std::array<libguarded::plain_guarded<size_t>,2> canStaticDataUsers;

    //can stuff
    // static std::array<can_frame,2> frame;
    // static std::array<ifreq, 2> ifr;
    // static std::array<sockaddr_can, 2> socketAddress;
    // static std::array<int,2> soc;

    // static std::array<bool,2> canBussesSetup;

    static bool setupCAN(int canBus);
    static void closeCAN(int canBus);

    static bool sendMSG(int canBus, can_frame frame);

public:
    CANDriver(int busNum);
    virtual ~CANDriver();


};

class SparkMax: CANDriver {
protected:
    int canID;
public:
    SparkMax(int canBUS, int canID);

    bool sendHeartbeat();

    void sendPowerCMD(float power);
    void ident();
};

class PWMSparkMax {
protected:
    int pin;

    static bool gpioSetup;
    static void setupGPIO();

    //init to an invalid value
    float lastSentValue = -10;

    bool initialSetup = false;

public:
    static void terminateGPIO();

    //must be a pwm pin
    PWMSparkMax(int pin);
    ~PWMSparkMax();

    void setPower(float power);
};
#endif
