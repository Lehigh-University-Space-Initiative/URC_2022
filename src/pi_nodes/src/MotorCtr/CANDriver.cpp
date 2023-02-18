#include "CANDriver.h"
#include "Utilities.h"
#include <string>

#define MAX_PWM 2000
#define MIN_PWM 1000

#define MAX_DUTY_CYCLE 255
#define MIN_DUTY_CYCLE 0

// std::array<can_frame,2> CANDriver::frame;
std::array<ifreq, 2> CANDriver::ifr;
std::array<sockaddr_can, 2> CANDriver::socketAddress;
std::array<int,2> CANDriver::soc;

std::array<bool,2> CANDriver::canBussesSetup;

bool CANDriver::setupCAN(int canBus)
{
    //check if already setup 
    if (canBussesSetup[canBus])
        return true;

    //see waveshare can hat demo .cpp file

    // memset(&(frame[canBus]), 0, sizeof(struct can_frame));

    std::string sudoCMD = "echo lusi | sudo -S ";

    system((sudoCMD + "ip link set can0 type can bitrate 100000").c_str());
    system((sudoCMD + "sudo ifconfig can0 up").c_str());

    //1.Create socket
    soc[canBus] = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (soc[canBus] < 0) {
        ROS_ERROR("socket PF_CAN failed");
        return false;
    }
    
    //2.Specify can0 device
    if (canBus)
        strcpy(ifr[canBus].ifr_name, "can1");
    else
        strcpy(ifr[canBus].ifr_name, "can0");

    int ret = ioctl(soc[canBus], SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        ROS_ERROR("ioctl failed");
        return false;
    }
    
    //3.Bind the socket to can0
    socketAddress[canBus].can_family = AF_CAN;
    socketAddress[canBus].can_ifindex = ifr[canBus].ifr_ifindex;
    ret = bind(soc[canBus], (struct sockaddr *)&(socketAddress[canBus]), sizeof(socketAddress));
    if (ret < 0) {
        perror("bind failed");
        return false;
    }
    
    //4.Disable filtering rules, do not receive packets, only send
    setsockopt(soc[canBus], SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    //setup can correctly

    canBussesSetup[canBus] = true;

    return true;
}

bool CANDriver::sendMSG(int canBus, const can_frame& frame)
{
    if (!canBussesSetup[canBus]) return false;

    //TODO: validation

    int nbytes = write(soc[canBus], &frame, sizeof(frame));  

    if(nbytes != sizeof(frame)) {
        ROS_ERROR("CAN Frame Send Error!\r\n");
        return false;
    }
    return true;
}

CANDriver::CANDriver(int busNum)
{
    assert(busNum < 2 && busNum >= 0);
    this->canBus = busNum;

    if (setupCAN(busNum)) {
        ROS_INFO("can setup suc");
    }
    else ROS_WARN("can not setup");
}

void CANDriver::closeCAN()
{
    close(soc[0]);
    close(soc[1]);

    system("sudo ifconfig can0 down");
    system("sudo ifconfig can1 down");

    canBussesSetup[0] = false;
    canBussesSetup[1] = false;
}

CANDriver::~CANDriver()
{
}


SparkMax::SparkMax(int canBUS, int canID)
    : CANDriver(canBUS), canID(canID)
{
}

void SparkMax::sendPowerCMD(float power)
{
    //safety limet
    power = min(max(power, -0.3f),0.3f);

    can_frame frame{}; 
    frame.can_id = 2050080 + canID;
    ROS_INFO(("sending can frame with id " + std::to_string(frame.can_id)).c_str());
    frame.can_dlc = 8;
    memcpy(frame.data,(int*)(&power), sizeof(float));
    auto temp = frame.data[0];
    frame.data[0] = frame.data[3];
    frame.data[3] = temp;

    temp = frame.data[1];
    frame.data[1] = frame.data[2];
    frame.data[2] = temp;

    ROS_INFO(("sending: " + std::to_string(power) + " which is " + std::to_string(frame.data[0]) + std::to_string(frame.data[1]) + std::to_string(frame.data[2]) + std::to_string(frame.data[3])).c_str());


    if (sendMSG(canBus, frame)) {
        ROS_INFO("Sent can message");
    }
    else ROS_WARN("Can message failed to send");
}

void PWMSparkMax::setupGPIO()
{
    if (gpioSetup) return;

    //setup pigpio
    if (gpioInitialise() < 0) {
        ROS_ERROR("pigpio initialisation failed");
        // return;
        throw std::runtime_error("pigpio initialisation failed");
    }


    gpioSetup = true;
}

PWMSparkMax::PWMSparkMax(int pin)
    : pin(pin)
{
    setupGPIO();
}

void PWMSparkMax::terminateGPIO()
{
    //terminate pigpio
    gpioTerminate();
}

bool PWMSparkMax::gpioSetup = false;



void PWMSparkMax::setPower(float power)
{
    //safety limet
    power = min(max(power, -0.6f),0.6f);

    //reinit lib if needed
    if (gpioInitialise() < 0) {
        ROS_ERROR("pigpio initialisation failed");
        // return;
        throw std::runtime_error("pigpio initialisation failed");
    }

    //use pwm to send power with pigpio

    //set pwm frequency to 100Hz
    auto freq = 100;
    gpioSetPWMfrequency(pin, freq);
    auto maxRange = 2000;
    gpioSetPWMrange(pin, maxRange); 

    // calcualte duty cycle from power
    //remap [-1,1] of the power to [1000,2000] of the duty cycle
    float pulseWidth = (power * 500 + 1500);

    //the dutyCycle needs to be between [0,255] (representing 0 and 100% respectivly)
    // int dutyCycle = (int)((MIN_PWM + (MAX_PWM - MIN_PWM) * (power + 1.0) / 2.0) * 255);
    //   int dutyCycle = (int)(MIN_DUTY_CYCLE + (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) * (power + 1.0) / 2.0);
    int dutyCycle = (int)(pulseWidth / 1000000 * freq * maxRange);


    ROS_INFO(("duty cycle: " + std::to_string(dutyCycle)).c_str());
    
    //set pwm range to 2500

    //set duty cycle 
    gpioPWM(pin, dutyCycle); 


}

PWMSparkMax::~PWMSparkMax()
{
    setPower(0);
}
