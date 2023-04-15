#!/usr/bin/env python

import sys
import rospy
import cv2
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
bridge = CvBridge()

# def read_cam():
#     cap = cv2.VideoCapture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1920, height=(int)1080,format=(string)NV12, framerate=(fraction)30/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert !  appsink")
#     if cap.isOpened():
#         cv2.namedWindow("demo", cv2.WINDOW_AUTOSIZE)
#         while True:
#             ret_val, img = cap.read();
#             cv2.imshow('demo',img)
#             cv2.waitKey(10)
#     else:
#      print "camera open failed"

frameNum = 0;

def main():
    rospy.init_node('VideoStreamer2')

    rospy.loginfo("videoStreamer2 is running")

    publisher = rospy.Publisher("/videoStream", Image)

    #TOOD: set to proper framerate
    rate = rospy.Rate(10);

    currentStreamingCam = -1

    cap=None

    def finishLoop():
        global frameNum
        frameNum += 1
        rate.sleep()


    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))
    while not rospy.is_shutdown():
        # if frameNum % 10 == 0:
            # if not rospy.has_param("/streamCam"):
            #     rospy.logwarn("No camera selected to stream")
            #     finishLoop()
            #     continue
            # newStreamingCam = rospy.get_param("/streamCam", 0)
            # if newStreamingCam != currentStreamingCam:
            #     currentStreamingCam = newStreamingCam
            #     cap = cv2.VideoCapture(newStreamingCam)
            #     if not cap.isOpened():
            #         rospy.logwarn("Requested cam does not exist")
            #         finishLoop()
            #         continue
            #     rospy.loginfo("got cam")

        ret_val, img = cap.read()

        if img is not None and not img.empty():
            rospy.loginfo("sending frame")
            msg = bridge.cv2_to_imgmsg(img, "bgr8")
            publisher.publish(msg)

        #finish loop

        finishLoop()
        

                

if __name__ == '__main__':
    main()