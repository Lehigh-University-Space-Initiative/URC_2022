#include "stdio.h"

#include "ros/ros.h"
#include <sstream>
#include "cross_pkg_messages/ManualDriveCMD.h"
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
#include "geometry_msgs/Vector3.h"

#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>



int main(int argc, char** argv) {
    
   ros::init(argc, argv, "VideoStreamer"); 

   ros::NodeHandle n;

   ROS_INFO("VideoStreamer is running");
   
   //TOOO: set rate to correct amount
   ros::Rate loop_rate(10 );//30.0); 


   int currentStreamingCam = -1;
   int newSteamingCam = 0;

   size_t frameNum = 0;

   image_transport::ImageTransport it(n);
   image_transport::Publisher pub = it.advertise("/videoStream", 1);

   cv::VideoCapture cap;
   cv::Mat frame;
   sensor_msgs::ImagePtr msg;

   while (n.ok()) {
      //proccsing

      //read in current streaming cam every 10 frames
      if (frameNum % 10 == 0) {
         if (!ros::param::has("/streamCam")) {
            ROS_WARN("No camera selected to stream");
            goto finishLoop;
         }
         ros::param::get("/streamCam",newSteamingCam);
         if (newSteamingCam != currentStreamingCam) {
            currentStreamingCam = newSteamingCam;
            cap = cv::VideoCapture(newSteamingCam);
            if(!cap.isOpened()) {
               ROS_WARN("Requested Cam does not exist");
               goto finishLoop;
            }
            ROS_INFO("got cam");
         }
      }

      //send frame
      ROS_INFO("requesting frame");
      cap >> frame;

      if(!frame.empty()) {
         ROS_INFO("sending frame");
         msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
         pub.publish(msg);
         cv::waitKey(1);
      }

      //run loop
      
      finishLoop:

      frameNum++;
      ros::spinOnce();
      loop_rate.sleep();
   }

}