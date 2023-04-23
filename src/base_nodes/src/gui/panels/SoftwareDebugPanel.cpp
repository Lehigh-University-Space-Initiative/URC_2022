#include "SoftwareDebugPanel.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

void SoftwareDebugPanel::drawBody()
{
    // Calculate time elapsed since last file read
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
    std::chrono::duration<double> timeElapsed = currentTime - lastFileReadTime;
    
    ImGui::Columns(4, "debugInfoColumns");
    ImGui::Text("Host");
    ImGui::NextColumn();
    ImGui::Text("Date and Time");
    ImGui::NextColumn();
    ImGui::Text("Git Branch");
    ImGui::NextColumn();
    ImGui::Text("Git User Name");
    ImGui::NextColumn();

    // Loop through hosts
    for (const auto& host : hosts) {
        // Read debug info file for host
        readDebugInfoFile(host);

        // Display host info
        ImGui::Text("Host: %s (%s)", host.hostName.c_str(), host.hostIpAddress.c_str());
        ImGui::NextColumn();

        // Display datetime, with elapsed time since first file read
        for (const auto& line : debugInfoLines) {
            std::istringstream iss(line);
            std::string dateTime, branch, userName;
            std::getline(iss, dateTime, ',');
            std::getline(iss, branch, ',');
            std::getline(iss, userName, ',');
            ImGui::Text("%s (%fs)", dateTime.c_str(), timeElapsed.count());
            ImGui::NextColumn();
            ImGui::Text("%s", branch.c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", userName.c_str());
            ImGui::NextColumn();
        }
        // Separator between hosts
        ImGui::Separator();
    }
    ImGui::Columns(1);
}

void SoftwareDebugPanel::setup()
{
    // Populate hosts array
    hosts = {
        {"192.168.0.1", "Host 1", "/home/tinapham/URC_2022/src/base_nodes/src/gui/panels/debugInfo.txt"},
        {"192.168.0.2", "Host 2", "/home/tinapham/URC_2022/src/base_nodes/src/gui/panels/debugInfo.txt"},
        {"192.168.0.3", "Host 3", "/home/tinapham/URC_2022/src/base_nodes/src/gui/panels/debugInfo.txt"}
    };
}

void SoftwareDebugPanel::update()
{
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
    if (currentTime - lastUpdateTime > refreshInterval) {
        for (auto& host : hosts) {
            readDebugInfoFile(host);
            ROS_INFO("Read debug info file for host %s (%s)", host.hostName.c_str(), host.hostIpAddress.c_str());
        }
        lastUpdateTime = currentTime;
    }
}

void SoftwareDebugPanel::readDebugInfoFile(const Host& host)
{
    std::ifstream file(host.debugFilePath);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> lines;
        std::getline(file, line); // ignore first line
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        debugInfoLines = std::move(lines);
        ROS_INFO("Read %d lines from debug info file for host %s (%s)", debugInfoLines, host.hostName.c_str(), host.hostIpAddress.c_str());
        file.close();        
    }
}
