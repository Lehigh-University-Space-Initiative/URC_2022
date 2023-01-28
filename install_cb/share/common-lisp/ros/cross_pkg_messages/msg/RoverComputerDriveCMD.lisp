; Auto-generated. Do not edit!


(cl:in-package cross_pkg_messages-msg)


;//! \htmlinclude RoverComputerDriveCMD.msg.html

(cl:defclass <RoverComputerDriveCMD> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (value
    :reader value
    :initarg :value
    :type geometry_msgs-msg:Vector3
    :initform (cl:make-instance 'geometry_msgs-msg:Vector3)))
)

(cl:defclass RoverComputerDriveCMD (<RoverComputerDriveCMD>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <RoverComputerDriveCMD>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'RoverComputerDriveCMD)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name cross_pkg_messages-msg:<RoverComputerDriveCMD> is deprecated: use cross_pkg_messages-msg:RoverComputerDriveCMD instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <RoverComputerDriveCMD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader cross_pkg_messages-msg:header-val is deprecated.  Use cross_pkg_messages-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'value-val :lambda-list '(m))
(cl:defmethod value-val ((m <RoverComputerDriveCMD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader cross_pkg_messages-msg:value-val is deprecated.  Use cross_pkg_messages-msg:value instead.")
  (value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <RoverComputerDriveCMD>) ostream)
  "Serializes a message object of type '<RoverComputerDriveCMD>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'value) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <RoverComputerDriveCMD>) istream)
  "Deserializes a message object of type '<RoverComputerDriveCMD>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'value) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<RoverComputerDriveCMD>)))
  "Returns string type for a message object of type '<RoverComputerDriveCMD>"
  "cross_pkg_messages/RoverComputerDriveCMD")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'RoverComputerDriveCMD)))
  "Returns string type for a message object of type 'RoverComputerDriveCMD"
  "cross_pkg_messages/RoverComputerDriveCMD")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<RoverComputerDriveCMD>)))
  "Returns md5sum for a message object of type '<RoverComputerDriveCMD>"
  "cb1dd5548a7b2dd376207dab58ce4eff")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'RoverComputerDriveCMD)))
  "Returns md5sum for a message object of type 'RoverComputerDriveCMD"
  "cb1dd5548a7b2dd376207dab58ce4eff")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<RoverComputerDriveCMD>)))
  "Returns full string definition for message of type '<RoverComputerDriveCMD>"
  (cl:format cl:nil "Header header~%geometry_msgs/Vector3 value~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Vector3~%# This represents a vector in free space. ~%# It is only meant to represent a direction. Therefore, it does not~%# make sense to apply a translation to it (e.g., when applying a ~%# generic rigid transformation to a Vector3, tf2 will only apply the~%# rotation). If you want your data to be translatable too, use the~%# geometry_msgs/Point message instead.~%~%float64 x~%float64 y~%float64 z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'RoverComputerDriveCMD)))
  "Returns full string definition for message of type 'RoverComputerDriveCMD"
  (cl:format cl:nil "Header header~%geometry_msgs/Vector3 value~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Vector3~%# This represents a vector in free space. ~%# It is only meant to represent a direction. Therefore, it does not~%# make sense to apply a translation to it (e.g., when applying a ~%# generic rigid transformation to a Vector3, tf2 will only apply the~%# rotation). If you want your data to be translatable too, use the~%# geometry_msgs/Point message instead.~%~%float64 x~%float64 y~%float64 z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <RoverComputerDriveCMD>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'value))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <RoverComputerDriveCMD>))
  "Converts a ROS message object to a list"
  (cl:list 'RoverComputerDriveCMD
    (cl:cons ':header (header msg))
    (cl:cons ':value (value msg))
))
