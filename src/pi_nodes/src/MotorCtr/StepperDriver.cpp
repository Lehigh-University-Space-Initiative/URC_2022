#include "StepperDriver.h"
#include "iostream"

// #define ROS_INFO()
// #define ROS_WARN()

void StepperDriver::setupGPIO()
{
    //if the gpio has already been setup, return
    if (initialSetup) {
        return;
    }
    
    //try to init gpio
    if (gpioInitialise() < 0) {
        // std::cout << "Failed to init gpio" << std::endl;
        return;
    }

    //set the pins to output
    gpioSetMode(dirPin, PI_OUTPUT);
    gpioSetMode(stepPin, PI_OUTPUT);
    gpioSetMode(enablePin, PI_OUTPUT);
    
    //set the pins to high
    gpioWrite(dirPin, PI_HIGH);
    gpioWrite(stepPin, PI_HIGH);
    gpioWrite(enablePin, PI_HIGH);

    //set the initial setup flag
    initialSetup = true;

    std::cout << "initiolized" << std::endl;
}

void StepperDriver::step()
{
    //set the step pin low
    gpioWrite(stepPin, PI_LOW);

    //wait 10 microseconds
    gpioDelay(1000);

    //set the step pin high
    gpioWrite(stepPin, PI_HIGH);

    std::cout << "stepped for step pin: " << stepPin << std::endl;
}

void StepperDriver::runStepTick()
{
    //using the last pulse time and velocity, determine if a step should be taken
    //remmber tick rate is 500Hz (every 2ms)
    //velocity is in rev/s

    //get the current time
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

    //get the time since the last step
    std::chrono::duration<float> timeSinceLastStep = currentTime - lastStepTime;

    
    //convert from ms to s
    auto timeDelta = timeSinceLastStep.count();
    std::cout << "time since last step is " << timeDelta << std::endl;

    //detemine if a step should be taken
    auto timeBetweenSteps = 1.0 / (abs(commandedVelocity) * stepsPerRev / gearRatio);
    std::cout << "time to step " << timeBetweenSteps << " vals for velocity, stepsPerRev and gear ratio " << commandedVelocity << " " << stepsPerRev << " " << gearRatio << std::endl;
    if (timeDelta > timeBetweenSteps) {
        //take a step
        step();

        //update the last step time
        lastStepTime = currentTime;
    }
}

StepperDriver::StepperDriver(int dirPin, int stepPin, int enablePin, float gearRatio, int stepsPerRev)
    : dirPin(dirPin), stepPin(stepPin), enablePin(enablePin), gearRatio(gearRatio), stepsPerRev(stepsPerRev) 
{
    setupGPIO();
    auto now = std::chrono::system_clock::now();

    lastCommandTime = now;
    lastStepTime = now;
    lastTickTime = now;
}

StepperDriver::~StepperDriver()
{
    //set dir and step pins to low
    gpioWrite(dirPin, PI_LOW);
    gpioWrite(stepPin, PI_LOW);

    //dont set enable pin to low, as it may be shared with other steppers
}

void StepperDriver::setEnable(bool enable)
{
    //set the enable pin
    gpioWrite(enablePin, enable ? PI_HIGH : PI_LOW);
}

bool StepperDriver::getEnable()
{
    //get if enable pin is high
    return gpioRead(enablePin) == PI_HIGH;
}

void StepperDriver::setDir(bool dir)
{
    if (dir == this->dir) {
        return;
    }
    //set the dir pin
    gpioWrite(dirPin, dir ? PI_HIGH : PI_LOW);

    this->dir = dir;
}

bool StepperDriver::getDir()
{
    return dir;
}

void StepperDriver::setVelocity(float velocity)
{
    float maxVelocity = 1;
    //clamp velocity to [-maxVelocity, maxVelocity]
    if (velocity > maxVelocity) {
        velocity = maxVelocity;
    } else if (velocity < -maxVelocity) {
        velocity = -maxVelocity;
    }

    commandedVelocity = velocity;

    //if commanded velocity turn dir on, else turn dir off
    if (commandedVelocity > 0) {
        setDir(true);
    } else if (commandedVelocity < 0) {
        setDir(false);
    }

    //if non zero velocity, and not enabled, print warning
    if (velocity != 0 && getEnable()) {
        // ROS_WARN("Setting velocity to non zero while not enabled");
    }
}

float StepperDriver::getVelocity()
{
    return commandedVelocity; 
}

void StepperDriver::tick()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> timeSinceLastStep = now - lastTickTime;
    // std::cout << "ticking with delta of " << timeSinceLastStep.count() << std::endl;

    //if the last command was more than 0.5 seconds ago, set velocity to 0
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastCommandTime).count() > 500) {
        // commandedVelocity = 0;
        std::cout << "reseting vel" << std::endl;
    }

    //run the step tick
    runStepTick();

    //update the last tick time
    lastTickTime = std::chrono::system_clock::now();
}


// pasting of header:
