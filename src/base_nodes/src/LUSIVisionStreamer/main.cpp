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

#include "LUSIVisionTelem.h"


libguarded::plain_guarded<std::vector<uchar>> lastImage;
libguarded::plain_guarded<std::vector<uchar>> lastImage2;

std::vector<uchar> no_img;

libguarded::plain_guarded<LUSIVIsionGenerator*> gen;

bool stereo = true;

void _proccess_frame(bool right_eye,const sensor_msgs::ImageConstPtr& msg) {
    //    ROS_INFO("got frame");

      cv::Mat img;

      auto im = cv_bridge::toCvShare(msg, "bgr8")->image;
      cv::resize(im, img, cv::Size(960, 540));

      std::vector<int> img_opts;

      img_opts.push_back(CV_IMWRITE_JPEG_QUALITY);
      img_opts.push_back(90);
      std::vector<uchar> img_dat;

      cv::imencode(".jpg",img,img_dat, img_opts);
      if (!right_eye) {
         auto last_img_dat = lastImage.lock();
         *last_img_dat = std::move(img_dat);

      } else {
         auto last_img_dat = lastImage2.lock();
         *last_img_dat = std::move(img_dat);
      }
      //ROS_INFO("Made img of size %d", img_dat->size());
      //auto now = std::chrono::system_clock::now();
      //ROS_INFO(("Frame Delta: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime).count())).c_str());


   }

void proccess_frame(const sensor_msgs::ImageConstPtr& msg) {
   _proccess_frame(false, msg);
}
void proccess_frame2(const sensor_msgs::ImageConstPtr& msg) {
   ROS_INFO("temp 2");
   _proccess_frame(true, msg);
}



void run_con(sockpp::tcp_socket sock, bool right_eye);

void run_tcp_server(bool right_eye) {
   std::error_code ec;
    sockpp::tcp_acceptor acc{!right_eye ? 8010 : 8011, 10, ec};

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
            ROS_INFO("Received a stream connection request from []", peer);
            numStreamClientsConnected++;
            sockpp::tcp_socket sock = res.release();

            // Create a thread and transfer the new stream to it.
            std::thread thr(run_con, std::move(sock),right_eye);
            thr.detach();
        }
    }

}

void run_con(sockpp::tcp_socket sock, bool right_eye) {

   ros::Rate loop(24);

   while (true) {

      if (!stereo) {
         auto img = lastImage.lock();

         LUSIStreamHeader head{};
         head.sizeFrame1 = img->size(); //img->rows * img->cols * 1 * 3;
         head.sizeFrame2 = 0;

         sock.write_n(&head,sizeof(head));
         //might be a problem: //count = 0;
         //sock.write_n(&count2,sizeof(count2)); //no second image
         auto res = sock.write_n(img->data(),head.sizeFrame1);

         if (res.value() <= 0) {
            break;
         }

      }
      else {
         auto img = lastImage.lock();
         auto img2 = lastImage2.lock();

         LUSIStreamHeader head{};
         head.sizeFrame1 = img->size(); //img->rows * img->cols * 1 * 3;
         head.sizeFrame2 = img2->size(); //img->rows * img->cols * 1 * 3;
         ROS_INFO("size1: %d,size2: %d",head.sizeFrame1, head.sizeFrame2);

         sock.write_n(&head,sizeof(head));
         auto res = sock.write_n(img->data(),head.sizeFrame1);
         auto res2 = sock.write_n(img2->data(),head.sizeFrame2);

         if (res.value() <= 0 || res2.value() < 0) {
            break;
         }

      }

      loop.sleep();
   }

   numStreamClientsConnected--;

}

//--------------TELEM----------------

void run_telem_con(sockpp::tcp_socket sock);

void run_telem_tcp_server() {
   std::error_code ec;
    sockpp::tcp_acceptor acc{8009, 10, ec};

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
            numClientsConnected++;
            sockpp::tcp_socket sock = res.release();

            // Create a thread and transfer the new stream to it.
            std::thread thr(run_telem_con, std::move(sock));
            thr.detach();
        }
    }

}

void run_telem_con(sockpp::tcp_socket sock) {

   std::vector<char> bytes;
   bytes.resize(10);
   
   ros::Rate loop(30);

   while (true)
   {

      LUSIVisionTelem telem;
      {
         auto g = gen.lock();
         telem = (*g)->generate();
      }
      auto res = sock.write_n(&telem, sizeof(telem));

      if (res.value() <= 0)
      {
         break;
      }

      loop.sleep();
   }

   numClientsConnected--;

}


// --------------------------------------------

ros::Subscriber sub;
ros::Subscriber sub2;



int main(int argc, char** argv) {
    
   ros::init(argc, argv, "LUSIVisionStreamer"); 

   ros::NodeHandle n;

   {
      auto g = gen.lock();
      *g = new LUSIVIsionGenerator(n);
   }

   //TOOO: set rate to correct amount
   ros::Rate loop_rate(60); 

   ROS_INFO("LUSI Vision Streamer is running");

    ROS_INFO("setup vid sub");
   sub = n.subscribe("/videoStream/image_raw", 1, proccess_frame);
   if (stereo)
      sub2 = n.subscribe("/videoStream2/image_raw", 1, proccess_frame2);


   auto no = cv::imread("/home/urcAssets/LUSNoDownlink.png");
   std::vector<int> img_opts;

   img_opts.push_back(CV_IMWRITE_JPEG_QUALITY);
   //img_opts.push_back(10);
   img_opts.push_back(10);

   cv::imencode(".jpg",no,no_img, img_opts);
   ROS_INFO("No: %d", no_img.size());


   //setup sockets
   sockpp::initialize();

   
   std::thread acceptor2;
   std::thread acceptor(run_tcp_server, false);
   // if (stereo)
   //    acceptor2 = std::thread(run_tcp_server, true);
   std::thread telem_acceptor(run_telem_tcp_server);
   
   while (ros::ok()) {
      //proccsing

      //run loop
      ros::spinOnce();
      loop_rate.sleep();
   }

   return 0;
}