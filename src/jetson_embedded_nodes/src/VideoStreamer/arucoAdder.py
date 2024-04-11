#!/usr/bin/env python

import sys
import rospy
import cv2
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
# import 
bridge = CvBridge()

arucoDict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
arucoParams = cv2.aruco.DetectorParameters()
detector = cv2.aruco.ArucoDetector(arucoDict,arucoParams)


rospy.init_node('VideoStreamer2')

rospy.loginfo("videoStreamer2 is running")

publisher = rospy.Publisher("/videoStreamAR", Image, queue_size=1)

def callback(data):
    frame = bridge.imgmsg_to_cv2(data);
    # (corners, ids, rejected) = cv2.aruco.detectMarkers(frame, arucoDict, parameters=arucoParams)
    corners, ids, rejected = detector.detectMarkers(frame)

    
    # verify ArUco marker was detected
    if len(corners) > 0:
        # rospy.loginfo("got some corners")
		# flatten the ArUco IDs list
        ids = ids.flatten()

		# loop over the detected ArUCo corners
        for (markerCorner, markerID) in zip(corners, ids):
			# extract the marker corners 
            corners = markerCorner.reshape((4, 2)) #idk what this reshape is
            (topLeft, topRight, bottomRight, bottomLeft) = corners

			# convert coords to integers
            topRight = (int(topRight[0]), int(topRight[1]))
            bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
            bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
            topLeft = (int(topLeft[0]), int(topLeft[1]))

            # draw the frame of the markers
            cv2.line(frame, topLeft, topRight, (0, 255, 0), 2)
            cv2.line(frame, topRight, bottomRight, (0, 255, 0), 2)
            cv2.line(frame, bottomRight, bottomLeft, (0, 255, 0), 2)
            cv2.line(frame, bottomLeft, topLeft, (0, 255, 0), 2)

			# drw the cnter of the marker
            cX = int((topLeft[0] + bottomRight[0]) / 2.0)
            cY = int((topLeft[1] + bottomRight[1]) / 2.0)
            cv2.circle(frame, (cX, cY), 4, (0, 0, 255), -1)

			# draw the ArUco marker ID on the frame
            cv2.putText(frame, str(markerID), (topLeft[0], topLeft[1] - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    msg = bridge.cv2_to_imgmsg(frame, encoding="bgr8")
    rospy.loginfo("Got frame")
    publisher.publish(msg)

sub = rospy.Subscriber("/videoStream", Image, callback)

rospy.spin();