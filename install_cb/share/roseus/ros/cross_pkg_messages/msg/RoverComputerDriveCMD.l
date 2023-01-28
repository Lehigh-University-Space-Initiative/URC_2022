;; Auto-generated. Do not edit!


(when (boundp 'cross_pkg_messages::RoverComputerDriveCMD)
  (if (not (find-package "CROSS_PKG_MESSAGES"))
    (make-package "CROSS_PKG_MESSAGES"))
  (shadow 'RoverComputerDriveCMD (find-package "CROSS_PKG_MESSAGES")))
(unless (find-package "CROSS_PKG_MESSAGES::ROVERCOMPUTERDRIVECMD")
  (make-package "CROSS_PKG_MESSAGES::ROVERCOMPUTERDRIVECMD"))

(in-package "ROS")
;;//! \htmlinclude RoverComputerDriveCMD.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass cross_pkg_messages::RoverComputerDriveCMD
  :super ros::object
  :slots (_header _value ))

(defmethod cross_pkg_messages::RoverComputerDriveCMD
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:value __value) (instance geometry_msgs::Vector3 :init))
    )
   (send-super :init)
   (setq _header __header)
   (setq _value __value)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:value
   (&rest __value)
   (if (keywordp (car __value))
       (send* _value __value)
     (progn
       (if __value (setq _value (car __value)))
       _value)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; geometry_msgs/Vector3 _value
    (send _value :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; geometry_msgs/Vector3 _value
       (send _value :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; geometry_msgs/Vector3 _value
     (send _value :deserialize buf ptr-) (incf ptr- (send _value :serialization-length))
   ;;
   self)
  )

(setf (get cross_pkg_messages::RoverComputerDriveCMD :md5sum-) "cb1dd5548a7b2dd376207dab58ce4eff")
(setf (get cross_pkg_messages::RoverComputerDriveCMD :datatype-) "cross_pkg_messages/RoverComputerDriveCMD")
(setf (get cross_pkg_messages::RoverComputerDriveCMD :definition-)
      "Header header
geometry_msgs/Vector3 value
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: geometry_msgs/Vector3
# This represents a vector in free space. 
# It is only meant to represent a direction. Therefore, it does not
# make sense to apply a translation to it (e.g., when applying a 
# generic rigid transformation to a Vector3, tf2 will only apply the
# rotation). If you want your data to be translatable too, use the
# geometry_msgs/Point message instead.

float64 x
float64 y
float64 z
")



(provide :cross_pkg_messages/RoverComputerDriveCMD "cb1dd5548a7b2dd376207dab58ce4eff")


