#include "TelemetryPanel.h"

void TelemetryPanel::drawBody()
{
    ImGui::Text("Left Motors");
    ImGui::ProgressBar(lastDriveCMD.CMD_L.x); 
    ImGui::ProgressBar(lastDriveCMD.CMD_L.y); 
    ImGui::ProgressBar(lastDriveCMD.CMD_L.z); 

    ImGui::Text("Right Motors");
    ImGui::ProgressBar(lastDriveCMD.CMD_R.x); 
    ImGui::ProgressBar(lastDriveCMD.CMD_R.y); 
    ImGui::ProgressBar(lastDriveCMD.CMD_R.z); 
}

void TelemetryPanel::setup()
{
    auto f = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
        this->lastDriveCMD = *p;
    });
    sub = n.subscribe("roverDriveCommands", 10, f);
}

void TelemetryPanel::update()
{
}

TelemetryPanel::~TelemetryPanel()
{
}
