
(cl:in-package :asdf)

(defsystem "cross_pkg_messages-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "ManualDriveCMD" :depends-on ("_package_ManualDriveCMD"))
    (:file "_package_ManualDriveCMD" :depends-on ("_package"))
    (:file "RoverComputerDriveCMD" :depends-on ("_package_RoverComputerDriveCMD"))
    (:file "_package_RoverComputerDriveCMD" :depends-on ("_package"))
  ))