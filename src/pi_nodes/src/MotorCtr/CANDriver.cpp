#include "CANDriver.h"
#include "Utilities.h"
#include <string>
#include "Limits.h"

#define MAX_PWM 2000
#define MIN_PWM 1000

#define MAX_DUTY_CYCLE 255
#define MIN_DUTY_CYCLE 0

std::array<CANDriver::CANStaticDataGuarded,2> CANDriver::canStaticData;
std::array<libguarded::plain_guarded<size_t>,2> CANDriver::canStaticDataUsers;
// std::array<can_frame,2> CANDriver::frame;
// std::array<ifreq, 2> CANDriver::ifr;
// std::array<sockaddr_can, 2> CANDriver::socketAddress;
// std::array<int,2> CANDriver::soc;

// std::array<bool,2> CANDriver::canBussesSetup;

bool CANDriver::setupCAN(int canBus)
{
    auto data = canStaticData[canBus].lock();


    //check if already setup 
    if (data->canBussesSetup)
        return true;

    //see waveshare can hat demo .cpp file

    // memset(&(frame[canBus]), 0, sizeof(struct can_frame));

    std::string sudoCMD = "echo lusi | sudo -S ";

    system((sudoCMD + "ip link set can0 type can bitrate 100000").c_str());
    system((sudoCMD + "sudo ifconfig can0 up").c_str());

    //1.Create socket
    data->soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    
    if (data->soc < 0) {
        ROS_ERROR("socket PF_CAN failed");
        return false;
    }
    
    //2.Specify can0 device
    if (canBus)
        strcpy(data->ifr.ifr_name, "can1");
    else
        strcpy(data->ifr.ifr_name, "can0");

    int ret = ioctl(data->soc, SIOCGIFINDEX, &(data->ifr));
    if (ret < 0) {
        ROS_ERROR("ioctl failed");
        return false;
    }
    
    //3.Bind the socket to can0
    data->socketAddress.can_family = AF_CAN;
    data->socketAddress.can_ifindex = data->ifr.ifr_ifindex;
    ret = bind(data->soc, (struct sockaddr *)&(data->socketAddress), sizeof(data->socketAddress));
    if (ret < 0) {
        perror("bind failed");
        return false;
    }
    
    //4.Disable filtering rules, do not receive packets, only send
    setsockopt(data->soc, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    //setup can correctly

    data->canBussesSetup = true;

    return true;
}

bool CANDriver::sendMSG(int canBus, const can_frame& frame)
{
    if (canBus < 0 || canBus > 1) return false;

    //Lock the data to get the socket
    auto data = canStaticData[canBus].lock();

    if (!data->canBussesSetup) return false;

    //TODO: validation

    int nbytes = write(data->soc, &frame, sizeof(frame));  

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
        // incriments the number of can busses using this can bus
        {
            auto data = canStaticDataUsers[busNum].lock();
            *data = *data + 1; 
        }
    }
    else ROS_WARN("can not setup");
}


void CANDriver::closeCAN(int canBus)
{
    if (canBus < 0 || canBus > 1) return;

    auto data = canStaticData[canBus].lock();

    if (!data->canBussesSetup) return;

    close(data->soc);

    if (canBus)
        system("sudo ifconfig can1 down");
    else
        system("sudo ifconfig can0 down");
    
    data->canBussesSetup = false;
}

CANDriver::~CANDriver()
{
    //decriments the number of can busses using this can bus
    {
        auto data = canStaticDataUsers[canBus].lock();
        *data = *data - 1; 

        if (*data == 0) {
            closeCAN(canBus);
        }
    }
}


SparkMax::SparkMax(int canBUS, int canID)
    : CANDriver(canBUS), canID(canID)
{
}

bool SparkMax::sendHeartbeat(int canBus)
{
    can_frame frame{};
    frame.can_id = 0x02052C80 + canID; 
    //data is all 1s
    frame.can_dlc = 8;
    for (int i = 0; i < 8; i++) {
        frame.data[i] = 0xFF;
    }

    return sendMSG(canBus, frame);
}

void SparkMax::sendPowerCMD(float power)
{
    //safety limet
    power = min(max(power, -MAX_DRIVE_POWER), MAX_DRIVE_POWER);

    //deadband
    if (abs(power) < DRIVE_DEADBAND) {
        power = 0;
    }

    can_frame frame{}; 
    frame.can_id = 0x2050080 + canID;
    frame.can_dlc = 6;
    memcpy(frame.data,(int*)(&power), sizeof(float));
    //last 2 bytes are 0
    frame.data[4] = 0;
    frame.data[5] = 0;
    
    // ROS_INFO(("sending: " + std::to_string(power) + " which is " + std::to_string(frame.data[0]) + std::to_string(frame.data[1]) + std::to_string(frame.data[2]) + std::to_string(frame.data[3])).c_str());


    if (sendMSG(canBus, frame)) {
        // ROS_INFO("Sent can message");
        return;
    }
    else ROS_WARN("Can Spark MAX Speed message failed to send");
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
    setPower(0);
}

void PWMSparkMax::terminateGPIO()
{
    //terminate pigpio
    gpioTerminate();
}

bool PWMSparkMax::gpioSetup = false;



void PWMSparkMax::setPower(float power)
{
    float limitPower = 0.8f;
    float deadZone = 0.08f;
    //safety limet
    power = min(max(power, -limitPower),limitPower);
    // if (abs(power) < deadZone) {power = 0;}

    if (power == lastSentValue) {
        return;
    }
    lastSentValue = power;


    //reinit lib if needed
    if (gpioInitialise() < 0) {
        ROS_ERROR("pigpio initialisation failed");
        // return;
        throw std::runtime_error("pigpio initialisation failed");
    }

    //use pwm to send power with pigpio

    //set pwm frequency to 100Hz
    auto freq = 100;
    auto maxRange = 2000;
    gpioSetPWMrange(pin, maxRange);
    if (!initialSetup) {
        gpioSetPWMfrequency(pin, freq);
        initialSetup = true;
    }

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
