#include "TelemetryPanel.h"

void TelemetryPanel::drawBody()
{
    ImGui::Text("Motor Inputs");

    ImGui::Separator();

    ImGui::Text("Left Motors");
    ImGui::ProgressBar(lastDriveCMD.CMD_L.x); 
    ImGui::ProgressBar(lastDriveCMD.CMD_L.y); 
    ImGui::ProgressBar(lastDriveCMD.CMD_L.z); 

    ImGui::Text("Right Motors");
    ImGui::ProgressBar(lastDriveCMD.CMD_R.x); 
    ImGui::ProgressBar(lastDriveCMD.CMD_R.y); 
    ImGui::ProgressBar(lastDriveCMD.CMD_R.z); 

    ImGui::Separator();

    ImGui::Text("Arm Inputs");

    ImGui::Text("Left Stick");

    ImGui::Text("Pitch");
    ImGui::SameLine();
    ImGui::ProgressBar(lastArmCMD.CMD_L.x *0.5 + 0.5);

    ImGui::Text("Base");
    ImGui::SameLine();
    ImGui::ProgressBar(lastArmCMD.CMD_L.y *0.5 + 0.5);

    ImGui::Separator();

    ImGui::Text("Right Stick");

    ImGui::Text("Elbow Pitch");
    ImGui::SameLine();
    ImGui::ProgressBar(lastArmCMD.CMD_R.x *0.5 + 0.5);

    ImGui::Text("Elbow Rotate");
    ImGui::SameLine();
    ImGui::ProgressBar(lastArmCMD.CMD_R.y *0.5 + 0.5);

    ImGui::Text("End Effector Rotate");
    ImGui::SameLine();
    ImGui::ProgressBar(lastArmCMD.CMD_R.z *0.5 + 0.5);
}

void TelemetryPanel::setup()
{
    auto f = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
        this->lastDriveCMD = *p;
    });
    auto f2 = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
        this->lastArmCMD = *p;
    });
    sub = n.subscribe("roverDriveCommands", 10, f);
    sub2 = n.subscribe("manualArmControl", 10, f2);
}

void TelemetryPanel::update()
{
}

TelemetryPanel::~TelemetryPanel()
{
}
