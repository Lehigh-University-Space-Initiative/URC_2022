#include "stdio.h"

#include "ros/ros.h"
#include <sstream>

#include <opencv2/opencv.hpp>
#include "sensor_msgs/image_encodings.h"
#include "sensor_msgs/CompressedImage.h"

#include <cv_bridge/cv_bridge.h>

#include "sockpp/udp_socket.h"
#include "sockpp/tcp_acceptor.h"

#include "cs_plain_guarded.h"
#include "thread"

libguarded::plain_guarded<std::vector<uchar>> lastImage;

std::vector<uchar> no_img;

void proccess_frame(const sensor_msgs::ImageConstPtr& msg) {
    //    ROS_INFO("got frame");

      cv::Mat img;

      auto im = cv_bridge::toCvShare(msg, "bgr8")->image;
      cv::resize(im, img, cv::Size(960, 540));

      std::vector<int> img_opts;

      img_opts.push_back(CV_IMWRITE_JPEG_QUALITY);
      img_opts.push_back(90);
      std::vector<uchar> img_dat;

      cv::imencode(".jpg",img,img_dat, img_opts);
      {
         auto last_img_dat = lastImage.lock();
         *last_img_dat = std::move(img_dat);

      }
      //ROS_INFO("Made img of size %d", img_dat->size());
      //auto now = std::chrono::system_clock::now();
      //ROS_INFO(("Frame Delta: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime).count())).c_str());


   }


void run_con(sockpp::tcp_socket sock);

void run_tcp_server() {
   std::error_code ec;
    sockpp::tcp_acceptor acc{8010, 4, ec};

    if (ec) {
        //cerr << "Error creating the acceptor: " << ec.message() << endl;
        ROS_ERROR("Could not create tcp acceptor");
        return;
    }
    //cout << "Awaiting connections on port " << port << "..." << endl;

    while (true) {
        sockpp::inet_address peer;

        // Accept a new client connection
        if (auto res = acc.accept(&peer); !res) {
            //cerr << "Error accepting incoming connection: " << res.error_message() << endl;
        }
        else {
            ROS_INFO("Received a connection request from []", peer);
            sockpp::tcp_socket sock = res.release();

            // Create a thread and transfer the new stream to it.
            std::thread thr(run_con, std::move(sock));
            thr.detach();
        }
    }

}

void run_con(sockpp::tcp_socket sock) {

   std::vector<char> bytes;
   bytes.resize(10);
   
   ros::Rate loop(30);

   while (true) {

      {
         auto img = lastImage.lock();

         int64_t count = img->size(); //img->rows * img->cols * 1 * 3;

         //ROS_INFO("Sending %d bytes",count);
         
         sock.write_n(&count,sizeof(count));
         sock.write_n(img->data(),count);

         //sock.write_n("H", 1);
         //ROS_INFO("H");
      }
      //sock.read(bytes.data(), bytes.size());

      loop.sleep();
   }

}


int main(int argc, char** argv) {
    
   ros::init(argc, argv, "LUSIVisionStreamer"); 

   ros::NodeHandle n;


   //TOOO: set rate to correct amount
   ros::Rate loop_rate(60); 

   ROS_INFO("LUSI Vision Streamer is running");

    ROS_INFO("setup vid sub");
   auto sub = n.subscribe("/videoStream/image_raw", 1, proccess_frame);


   auto no = cv::imread("/home/urcAssets/LUSNoDownlink.png");
   std::vector<int> img_opts;

   img_opts.push_back(CV_IMWRITE_JPEG_QUALITY);
   img_opts.push_back(10);

   cv::imencode(".jpg",no,no_img, img_opts);
   ROS_INFO("No: %d", no_img.size());


   //setup sockets
   sockpp::initialize();

   std::thread acceptor(run_tcp_server);
   
   while (ros::ok()) {
      //proccsing

      //run loop
      ros::spinOnce();
      loop_rate.sleep();
   }

   return 0;
}