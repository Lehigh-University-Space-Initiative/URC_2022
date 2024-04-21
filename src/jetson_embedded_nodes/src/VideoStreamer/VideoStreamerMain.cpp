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

   std::array<int,4> camMap = {0,1,2,3};
   int cam3D = 1;

   ROS_INFO("VideoStreamer is running");
   
   //TOOO: set rate to correct amount
   ros::Rate loop_rate(30);//30.0); 


   int currentStreamingCam = -1;
   int newSteamingCam = 0;

   size_t frameNum = 0;

   image_transport::ImageTransport it(n);
   image_transport::Publisher pub = it.advertise("/videoStream", 1);
   image_transport::Publisher pub2 = it.advertise("/videoStream2", 1);

   cv::VideoCapture cap;
   cv::VideoCapture cap2;
   cv::Mat frame;
   cv::Mat frame2;
   sensor_msgs::ImagePtr msg;
   sensor_msgs::ImagePtr msg2;

   std::chrono::system_clock::time_point lastFrameSentTime;

   int lusi_vision_3d = false;
   int new_lusi_vision_3d = false;
   bool firstRun = true;

   while (n.ok()) {
      
    

      //read in current streaming cam every 10 frames
      if (frameNum % 1 == 0) {
         if (!ros::param::has("/streamCam") || !ros::param::has("/lusi_vision_mode")) {
            ROS_WARN("No camera selected to stream");
            // goto finishLoop;
            frameNum++;
            ros::spinOnce();
            loop_rate.sleep();
            continue;
         }

         ros::param::get("/lusi_vision_mode", new_lusi_vision_3d);
         if (new_lusi_vision_3d != lusi_vision_3d || firstRun)
         {
            //ROS_INFO("New 3D Cam Mode: %d, Got: %d, has: %d", new_lusi_vision_3d, got, ros::param::has("/lusi_vision_mode"));
            firstRun = false;
            lusi_vision_3d = new_lusi_vision_3d;
            if (lusi_vision_3d)
            {
               cap2 = cv::VideoCapture(cam3D, CV_CAP_V4L2);
               if (!cap.isOpened())
               {
                  ROS_WARN("Requested Cam 3D does not exist");
               } else {
                  ROS_INFO("Got 3D Cam");
               }
            } else {
               cap2 = cv::VideoCapture();
               ROS_INFO("Cleared 3D Cam");
            }
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

      if (lusi_vision_3d) {
         cap2 >> frame2;
      } else {
         frame2 = cv::Mat();
      }

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

      // if(!frame2.empty()) {

      //    // if (currentStreamingCam == 1) {
      //    //    cv::flip(frame,frame, 0);
      //    // }

      //    msg2 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame2).toImageMsg();
      //    // auto now = std::chrono::system_clock::now();
      //    // auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameSentTime).count();
      //    // //lastFrameSentTime = now;
      //    // ROS_INFO(("sending frame 2 with delta: " + std::to_string(delta)).c_str());
      //    pub2.publish(msg2);
      //    //cv::waitKey(1);
      // } else if (lusi_vision_3d) {
      //    ROS_INFO("no frame data 2");
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

      //ROS_INFO("asdasdasdklasdjaskldfjasklfasjf");

      

      //run loop
      
      finishLoop:

      frameNum++;
      ros::spinOnce();
      loop_rate.sleep();
   }

}