#include "SystemControlPanel.h"

void SystemControlPanel::drawBody() {
    // ImGui::Text("Left Motors");
    // ImGui::ProgressBar(lastDriveCMD.CMD_L.x); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_L.y); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_L.z); 

    // ImGui::Text("Right Motors");
    // ImGui::ProgressBar(lastDriveCMD.CMD_R.x); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_R.y); 
    // ImGui::ProgressBar(lastDriveCMD.CMD_R.z); 
    
    //E-Stop Button and Parameter:
    if (eStopStatus) {  //switch to red (e-stop on)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    else {  //Switch to green (e-stop off)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    }

    if (ImGui::Button("Toggle ESTOP")) {
        eStopStatus = !eStopStatus;   
    }
    
    n.setParam("/eStopStatus", eStopStatus);  //Will be used in other nodes

    ImGui::PopStyleColor();

    //Reboot and Deploy Code buttons:
    if (ImGui::Button("Reboot")) {
        // system("~/URC_2022/URC_DeployTools/reboot.sh");
    }
    
    ImGui::SameLine();

    if (ImGui::Button("Deploy Code")) {}
    
    ImGui::SameLine();
}

void SystemControlPanel::setup()
{
//     auto f = boost::function<void(const cross_pkg_messages::RoverComputerDriveCMDConstPtr&)>([this](auto p){
//         this->lastDriveCMD = *p;
//     });
//     sub = n.subscribe("roverDriveCommands", 10, f);
}


SystemControlPanel::~SystemControlPanel() {}