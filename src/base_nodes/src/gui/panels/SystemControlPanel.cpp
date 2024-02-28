#include "SystemControlPanel.h"

void SystemControlPanel::drawBody()
{
    // ImGui::Text("Left Motors");
    // ImGui::ProgressBar(lastDriveCMD.CMD_L.x); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_L.y); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_L.z); 

    // ImGui::Text("Right Motors");
    // ImGui::ProgressBar(lastDriveCMD.CMD_R.x); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_R.y); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_R.z); 

    //software locks all motors to stop
    if (ImGui::Button("Motor Freeze")) {
        
    }

    if (ImGui::Button("Reboot")) {
        system("~/URC_2022/URC_DeployTools/reboot.sh");
    }

    ImGui::SameLine();

    if (ImGui::Button("Deploy Code")) {
        
    }
    
    ImGui::SameLine();

    if (ImGui::Button("Close Ground Station")) {
        close_ui = true;
    }

}

void SystemControlPanel::setup()
{
//     auto f = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
//         this->lastDriveCMD = *p;
//     });
//     sub = n.subscribe("roverDriveCommands", 10, f);
}

void SystemControlPanel::update()
{

}

SystemControlPanel::~SystemControlPanel()
{

}
