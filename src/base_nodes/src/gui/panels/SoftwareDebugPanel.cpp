#include "SoftwareDebugPanel.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

void SoftwareDebugPanel::drawBody()
{
    ImGui::Columns(6, "debugInfoColumns");
    ImGui::Text("Date and Time");
    ImGui::NextColumn();
    ImGui::Text("Git Commit Hash");
    ImGui::NextColumn();
    ImGui::Text("Git Commit Message");
    ImGui::NextColumn();
    ImGui::Text("Git Branch");
    ImGui::NextColumn();
    ImGui::Text("Git User Name");
    ImGui::NextColumn();
    ImGui::Text("Git User Email");
    ImGui::NextColumn();
    ImGui::Separator();

    for (const auto& line: debugInfoLines) {
        std::istringstream iss(line);
        std::string dateTime, commitHash, commitMessage, branch, userName, userEmail;
        std::getline(iss, dateTime, ',');
        std::getline(iss, commitHash, ',');
        std::getline(iss, commitMessage, ',');
        std::getline(iss, branch, ',');
        std::getline(iss, userName, ',');
        std::getline(iss, userEmail, ',');
        ImGui::Text("%s", dateTime.c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", commitHash.c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", commitMessage.c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", branch.c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", userName.c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", userEmail.c_str());
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
}

void SoftwareDebugPanel::setup()
{
}

void SoftwareDebugPanel::update()
{
    double currentTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (currentTime - lastUpdateTime > refreshInterval) {
        readDebugInfoFile("debugInfo.txt");
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
        file.close();
    }
}
