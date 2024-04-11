
#include <libgpsmm.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <gps.h>

#include "ros/ros.h"

#include "cross_pkg_messages/GPSData.h"
#include "cross_pkg_messages/RoverComputerDriveCMD.h"


int main(int argc, char** argv) {
    ros::init(argc, argv, "GPS_Driver"); 
    ros::NodeHandle n;

    ROS_INFO("GPS Driver");

    auto gpsPublisher = n.advertise<cross_pkg_messages::GPSData>("/gps_data", 10);

   ros::Rate loop_rate(30);

    gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

  if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == nullptr) {
    std::cerr << "No GPSD running.\n";
    return 1;
  }

  constexpr auto kWaitingTime{1000000};  // 1000000 is 1 second

  for (;;) {
    if (!gps_rec.waiting(kWaitingTime)) {
      continue;
    }

    struct gps_data_t* gpsd_data;

    if ((gps_rec.read()) == nullptr) {
      ROS_ERROR("GPSD read error.");
      std::cerr << "GPSD read error.\n";
      return 1;
    }

    while (((gpsd_data = gps_rec.read()) == nullptr) || (gpsd_data->fix.mode < MODE_2D)) {
      // Do nothing until fix, block execution for 1 second (busy wait mitigation)
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    const auto latitude{gpsd_data->fix.latitude};
    const auto longitude{gpsd_data->fix.longitude};
    const auto alt(gpsd_data->fix.altitude);
    const auto speed(gpsd_data->fix.speed);
    const auto course(gpsd_data->fix.track);
    
    const auto sats(gpsd_data->satellites_used);
    const auto lat_acc(gpsd_data->fix.epy);
    const auto lon_acc(gpsd_data->fix.epx);
    const auto alt_acc(gpsd_data->fix.epv);

    cross_pkg_messages::GPSData gpsData;
    gpsData.status = gpsd_data->status;
    gpsData.lla.x = latitude;
    gpsData.lla.y = longitude;
    gpsData.lla.z = alt;
    gpsData.speed = speed;
    gpsData.course = course;

    gpsData.sats = sats;
    gpsData.lla_acc.x = lat_acc;
    gpsData.lla_acc.y = lon_acc;
    gpsData.lla_acc.z = alt_acc;

    gpsPublisher.publish(gpsData);

  }

}