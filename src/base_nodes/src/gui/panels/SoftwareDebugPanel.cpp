#include "SoftwareDebugPanel.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

// Command: sshpass -p lusi ssh pi@192.168.1.5 cat /home/pi/urc_deploy/debugInfo.csv
// Command: sshpass -p {pass} ssh {user}@{ip} cat {path}
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

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

        // Display host info
        ImGui::Text("Host: %s (%s)", host.hostName.c_str(), host.hostIpAddress.c_str());
        ImGui::NextColumn();

        // Display datetime, with elapsed time since first file read
        for (const auto& line : debugInfoLines) {
            std::istringstream iss(line);
            std::string dateTime, branch, userName;
            std::string gitCommitHash, gitCommitMessage; // don't need these
            std::getline(iss, dateTime, ',');
            std::getline(iss, gitCommitHash, ',');
            std::getline(iss, gitCommitMessage, ',');
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
        {"192.168.0.1", "Host 1", "/home/pi/urc_deploy/debugInfo.csv", "pi", "lusi", {}},
        {"192.168.0.2", "Host 2", "/home/pi/urc_deploy/debugInfo.csv", "pi", "lusi", {}},
        {"192.168.0.3", "Host 3", "/home/pi/urc_deploy/debugInfo.csv", "pi", "lusi", {}}
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

// Command: sshpass -p lusi ssh -o ConnectTimeout=2 pi@192.168.0.1 cat /home/pi/urc_deploy/debugInfo.csv
// Command: sshpass -p {pass} ssh -o ConnectTimeout=2 {user}@{ip} cat {path}
void SoftwareDebugPanel::readDebugInfoFile(const Host& host)
{
    std::string cmd = "sshpass -p " + host.password + " ssh -o ConnectTimeout=2 " + host.user + "@" + host.hostIpAddress + " cat " + host.debugFilePath;
    ROS_INFO("Executing command: %s", cmd.c_str());
    std::string result = exec(cmd.c_str());

    std::size_t found = result.find_last_of("/\\");
    host.debugInfoLines.push_back(result.substr(found+1));
    ROS_INFO("Read the last line from debug info file for host %s (%s)", host.hostName.c_str(), host.hostIpAddress.c_str());
}
