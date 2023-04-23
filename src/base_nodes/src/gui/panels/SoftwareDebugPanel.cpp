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
    
    ImGui::Columns(3, "debugInfoColumns");
    // ImGui::Text("Date and Time");
    // ImGui::NextColumn();
    // ImGui::Text("Git Commit Hash");
    // ImGui::NextColumn();
    // ImGui::Text("Git Commit Message");
    // ImGui::NextColumn();
    // ImGui::Text("Git Branch");
    // ImGui::NextColumn();
    // ImGui::Text("Git User Name");
    // ImGui::NextColumn();
    // ImGui::Text("Git User Email");
    // ImGui::NextColumn();
    // ImGui::Separator();

    for (const auto& line: debugInfoLines) {
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
    ImGui::Columns(1);
}

void SoftwareDebugPanel::setup()
{
}

void SoftwareDebugPanel::update()
{
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
    if (currentTime - lastUpdateTime > refreshInterval) {
        readDebugInfoFile("/home/tinapham/URC_2022/src/base_nodes/src/gui/panels/debugInfo.txt");
        ROS_INFO("Read debug info file");
        lastUpdateTime = currentTime;
    }
}

void SoftwareDebugPanel::readDebugInfoFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        debugInfoLines = std::move(lines);
        ROS_INFO("Read %d lines from debug info file", debugInfoLines);
        file.close();        
    }
}
