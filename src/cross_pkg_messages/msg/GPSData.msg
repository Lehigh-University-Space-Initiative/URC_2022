Header header

#uint8 STATUS_NO_FIX = 0
#uint8 STATUS_FIX_NO_ALT = 1
#uint8 STATUS_FIX_WITH_ALT = 2

uint8 status
geometry_msgs/Vector3 lla
float64 speed
float64 course

int32 sats
geometry_msgs/Vector3 lla_acc