#pragma once
#include "../Panel.h"
#include "../GUI.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <ros/ros.h>
#include <rosgraph_msgs/Log.h>
#include <ros/console.h>

// this will be a panel which streams the ROS logging console to the window

class Console : public Panel
{
protected:
    int log_level_filter_index = 0;
    const char *log_level_filter_options[5] = {"Debug", "Info", "Warn", "Error", "Fatal"};
    ros::Subscriber sub;
    ros::NodeHandle nh;

    struct LogMsg
    {
        int level;
        std::string levelString;
        std::string msg;
        std::string node;
        std::string completeOutput;
    };

    std::vector<LogMsg> filtered_log_msgs;

public:
    virtual void drawBody();
    virtual void callback(const rosgraph_msgs::Log::ConstPtr &msg);
    using Panel::Panel;
    virtual void setup() override;
    virtual void update() override;
    ~Console();
};
