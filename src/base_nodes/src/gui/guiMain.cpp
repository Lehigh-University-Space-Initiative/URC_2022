#include "stdio.h"

#include "ros/ros.h"
#include <sstream>
#include "cross_pkg_messages/ManualDriveCMD.h"
#include "sensor_msgs/Joy.h"

#include "GUI.h"
#include "Lifecycle.h"
#include "Panel.h"

#include "panels/ComStatusPanel.h"
#include "panels/TelemetryPanel.h"
#include "panels/SystemControlPanel.h"
#include "panels/VideoViewPanel.h"
#include "panels/SoftwareDebugPanel.h"

std::vector<Panel*> uiPanels{};

void loadPanels() {
   uiPanels.push_back(new ComStatusPanel("COMS Status"));
   uiPanels.push_back(new TelemetryPanel("Telemetry"));
   uiPanels.push_back(new SystemControlPanel("System Control"));
   uiPanels.push_back(new VideoViewPanel("Video Stream"));
   uiPanels.push_back(new SoftwareDebugPanel("Software Debug"));

   for(auto p : uiPanels) {
      p->setup();
   }
}


int main(int argc, char** argv) {
    
   ros::init(argc, argv, "GroundStationGUI"); 

   ros::NodeHandle n;

   loadPanels();

   ROS_INFO("GroundStationGUI is running");

   auto window = setupIMGUI(); 

   ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

   ros::Rate sleepRate(120);

   while (n.ok() && !glfwWindowShouldClose(window)) {
      glfwPollEvents();

      //ros updates
      ros::spinOnce();
      for (auto pan : uiPanels)
         pan->update();

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      //render frame

      for (auto pan : uiPanels)
         pan->renderToScreen();

      renderFrame(window, clear_color);

      sleepRate.sleep();
   }
   ROS_INFO("starting down gui");

   //cleanup
   {
      // Cleanup
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();

      glfwDestroyWindow(window);
      glfwTerminate();
   }

   ROS_INFO("Shutting down gui");

   n.shutdown();

   return 0;
}