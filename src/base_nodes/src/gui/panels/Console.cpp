#include "Console.h"

void Console::drawBody()
{
    ImGui::Begin("Console");
    ImGui::Text("ROS Console Output");

    if (ImGui::BeginCombo("Log Level Filter", log_level_filter_options[log_level_filter_index]))
    {

        for (int i = 0; i < 5; i++)
        {
            bool is_selected = (log_level_filter_index == i);
            if (ImGui::Selectable(log_level_filter_options[i], is_selected))
            {
                log_level_filter_index = i;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    for (int i = 0; i < filtered_log_msgs.size(); i++)
    {
        if (filtered_log_msgs[i].level >= log_level_filter_index)
        {
            ImGui::Text(filtered_log_msgs[i].completeOutput.c_str());
        }
    }

    ImGui::End();
}

void Console::callback(const rosgraph_msgs::Log::ConstPtr &msg)
{
    LogMsg log_msg;
    log_msg.node = msg->name;
    log_msg.msg = msg->msg;

    switch (msg->level)
    {
    case rosgraph_msgs::Log::DEBUG:
        log_msg.level = 0;
        log_msg.levelString = "DEBUG";
        break;
    case rosgraph_msgs::Log::INFO:
        log_msg.level = 1;
        log_msg.levelString = "INFO";
        break;
    case rosgraph_msgs::Log::WARN:
        log_msg.level = 2;
        log_msg.levelString = "WARN";
        break;
    case rosgraph_msgs::Log::ERROR:
        log_msg.level = 3;
        log_msg.levelString = "ERROR";
        break;
    case rosgraph_msgs::Log::FATAL:
        log_msg.level = 4;
        log_msg.levelString = "FATAL";
        break;
    default:
        log_msg.level = 0;
        log_msg.levelString = "DEBUG";
        break;
    }

    log_msg.completeOutput = "[" + log_msg.levelString + "] " + log_msg.node + ": " + log_msg.msg;
    filtered_log_msgs.push_back(log_msg);
}

void Console::setup()
{
    auto f = boost::function<void(const rosgraph_msgs::Log::ConstPtr &)>([this](auto p)
                                                                         { this->callback(p); });
    sub = nh.subscribe("/rosout", 1000, f);
}

void Console::update()
{
    ros::spinOnce();
}

Console::~Console()
{
    ros::shutdown();
}
