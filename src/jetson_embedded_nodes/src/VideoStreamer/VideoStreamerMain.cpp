#include "stdio.h"

#include "ros/ros.h"
#include <sstream>
#include "cross_pkg_messages/ManualDriveCMD.h"
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
#include "geometry_msgs/Vector3.h"

#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/videoio/videoio_c.h>

#include <chrono>
// #include <opencv2/aruco.hpp>

void findArucus(cv::Mat img) {
   
}

// void addArucoTo

int main(int argc, char** argv) {
    
   ros::init(argc, argv, "VideoStreamer"); 

   ros::NodeHandle n;

   std::array<int,4> camMap = {0,1,2,99};

   ROS_INFO("VideoStreamer is running");
   
   //TOOO: set rate to correct amount
   ros::Rate loop_rate(30);//30.0); 


   int currentStreamingCam = -1;
   int newSteamingCam = 0;

   size_t frameNum = 0;

   image_transport::ImageTransport it(n);
   image_transport::Publisher pub = it.advertise("/videoStream", 1);

   cv::VideoCapture cap;
   cv::Mat frame;
   sensor_msgs::ImagePtr msg;

   std::chrono::system_clock::time_point lastFrameSentTime;

   while (n.ok()) {
      //proccsing

      //read in current streaming cam every 10 frames
      if (frameNum % 1 == 0) {
         if (!ros::param::has("/streamCam")) {
            // ROS_WARN("No camera selected to stream");
            // goto finishLoop;
            frameNum++;
            ros::spinOnce();
            loop_rate.sleep();
            continue;
         }
         ros::param::get("/streamCam",newSteamingCam);
         if (newSteamingCam != currentStreamingCam) {
            currentStreamingCam = newSteamingCam;
            if (newSteamingCam >= camMap.size()) {
               ROS_WARN("Invalid cam num");
                           frameNum++;
            ros::spinOnce();
            loop_rate.sleep();
            continue;
            }
            auto actualCam = camMap[newSteamingCam];
            cap = cv::VideoCapture(actualCam, CV_CAP_V4L2);
            // cap.set(CV_CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));
            // cap.set(CV_CAP_PROP_FRAME_HEIGHT, 540);
            // cap.set(CV_CAP_PROP_FRAME_WIDTH, 960);
            if(!cap.isOpened()) {
               ROS_WARN("Requested Cam does not exist");
               // goto finishLoop;
            frameNum++;
            ros::spinOnce();
            loop_rate.sleep();
            continue;
            }
            ROS_INFO("got cam");
         }
      }

      //send frame
      // ROS_INFO("requesting frame");
      cap >> frame;

      //if aruco do that
      bool processAruco = false;

      // if (processAruco) {
      //    std::vector<int> markerIds;
      //    std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

      //    cv::aruco::DetectorParameters detectionParams = cv::aruco::DetectorParameters(); 
      //    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
      //    // cv::aruco::ArucoDetector detector(dictionary,detectionParams);
      //    cv::aruco::detectMarkers(frame,&dictionary,markerCorners,markerIds,&detectionParams,rejectedCandidates);


      // }

      if(!frame.empty()) {

         // if (currentStreamingCam == 1) {
         //    cv::flip(frame,frame, 0);
         // }

         msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
         auto now = std::chrono::system_clock::now();
         auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameSentTime).count();
         lastFrameSentTime = now;
         // ROS_INFO(("sending frame with delta: " + std::to_string(delta)).c_str());
         pub.publish(msg);
         cv::waitKey(1);
      } else {
         ROS_INFO("no frame data");
      }

      //run loop
      
      finishLoop:

      frameNum++;
      ros::spinOnce();
      loop_rate.sleep();
   }

}